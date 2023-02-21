#include "term_ssh.h"
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "netdb.h" // gethostbyname

#include "libssh2_config.h"
#include <libssh2.h>
#include <libssh2_sftp.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

static const char *TAG = "SSH";

const char *ssh_err_msg[] = {
    "OK", "HANDSHAKE", "PASSWORD", "CHANNEL", "SHELL", "READ"
};

extern EventGroupHandle_t xEventGroup;
extern int TASK_FINISH_BIT;

static void disconn_fire_cb(el_event_t *event)
{
    term_ssh_t *ssh = event->payload.user_data1;
    ssh->conn_cb(ssh, event->payload.user_data3);
}

static void post_conn_event(term_ssh_t *ssh, int err)
{
    el_event_t *event = el_event_create(disconn_fire_cb);
    event->payload.user_data1 = ssh;
    event->payload.user_data3 = err;
    el_looper_post_event(ssh->looper, event);
}

#if 0
struct recv_pack_t {
    term_ssh_t *ssh;
    void *buffer;
    int len;
    el_free_cb_t free_cb;
};

static void recv_fire_cb(el_event_t *event)
{
    recv_pack_t *pack = event->payload.user_data1;
    term_ssh_t *ssh = pack->ssh;
    ssh->recv_cb(ssh, pack->buffer, pack->len);
}

static void pack_free(void *ptr)
{
    el_event_t *event = ptr;
    recv_pack_t *pack = event->payload.user_data1;
    el_mem_free(pack->buffer);
    pack->free_cb(event);
    el_mem_free(pack);
}

static void post_recv_event(term_ssh_t *ssh, char *buffer, int len)
{
    recv_pack_t *pack = el_mem_malloc(sizeof(*pack));
    el_event_t *event = el_event_create(recv_fire_cb);

    void *new_buffer = el_mem_malloc(len);
    memcpy(new_buffer, buffer, len);
    event->payload.user_data1 = pack;

    pack->free_cb = event->free_cb;
    pack->ssh = ssh;
    pack->buffer = new_buffer;
    pack->len = len;
    event->free_cb = pack_free;
    el_looper_post_event(ssh->looper, event);
}
#else
static void recv_fire_cb(el_event_t *event)
{
    term_ssh_t *ssh = event->payload.user_data1;
    ssh->recv_cb(ssh, NULL, 0);
    ssh->read_event = NULL;
}

static void post_recv_event(term_ssh_t *ssh)
{
    el_event_t *event = el_event_create(recv_fire_cb);
    event->payload.user_data1 = ssh;
    ssh->read_event = event;
    el_looper_post_event(ssh->looper, event);
    while(ssh->read_event)
    {
        el_sleep_ms(3);
    }
}
#endif

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    /* now make sure we wait in the correct direction */
    dir = libssh2_session_block_directions(session);
    printf("dir %d\n", dir);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    //if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
    //    writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
    return rc;
}


static void *ssh_task(void *param)
{
    int rev;
    term_ssh_t *ssh = param;
    int sock = ssh->sock;
    LIBSSH2_SESSION *session = ssh->session;
    LIBSSH2_CHANNEL *channel;

    ESP_LOGI(TAG, "Task ssh:%p session:%p sock:%d\n", ssh, session, sock);

    ESP_LOGI(TAG, "session:%p  sock:%d", session, sock);
    rev = libssh2_session_handshake(session, sock);
    if(rev)
    {
        ESP_LOGE(TAG, "Failure establishing SSH session: %d", rev);
        post_conn_event(ssh, SSH_ERR_HANDSHAKE);
        return NULL;
    }
    ESP_LOGI(TAG, "hand shake over");

    if(libssh2_userauth_password(session, ssh->user, ssh->pwd))
    {
        ESP_LOGE(TAG, "Authentication usr:%s pwd:%s failed.", ssh->user, ssh->pwd);
        post_conn_event(ssh, SSH_ERR_PASSWORD);
        goto err;
    }
    libssh2_trace(session, LIBSSH2_TRACE_SOCKET);

    while((channel = libssh2_channel_open_session(session)) == NULL &&
          libssh2_session_last_error(session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN) 
    {
        waitsocket(sock, session);
    }
    if(channel == NULL) {
        ESP_LOGI(TAG, "libssh2_channel_open_session failed.");
        post_conn_event(ssh, SSH_ERR_CHANNEL);
        goto err;
    }
    ssh->channel = channel;

    libssh2_channel_request_pty(channel, "xterm");
    libssh2_channel_request_pty_size(channel, 40, 15);

    while((rev = libssh2_channel_shell(channel)) == LIBSSH2_ERROR_EAGAIN)
    {
	    waitsocket(sock, session);
    }
    if(rev != 0) {
        ESP_LOGI(TAG, "libssh2_channel_exec failed: %d", rev);
        post_conn_event(ssh, SSH_ERR_SHELL);
        goto err;
    }
    
    char *cmd;
    cmd = "uname && tty\n";
    libssh2_channel_write(channel, cmd, strlen(cmd));

    post_conn_event(ssh, SSH_ERR_OK);

    while (!ssh->recv_cb)
    {
        el_sleep_ms(3);
    }

    {
        int flags;
        flags = fcntl(ssh->sock, F_GETFL, 0);
        if(flags < 0)
        {
            perror("fcntl");
            goto err;
        }
        flags |= O_NONBLOCK;
        if(fcntl(ssh->sock, F_SETFL, flags) < 0)
        {
            perror("fcntl");
            goto err;
        }
    }

    while (1)
    {
        rev = waitsocket(ssh->sock, ssh->session);
        printf("libssh2 select:%d\n", rev);
        if (rev > 0)
        {
            post_recv_event(ssh);
        }
        else if (rev == 0)
        {

        }
        else
        {
            post_conn_event(ssh, SSH_ERR_READ);
            break;
        }
    }

    vTaskDelete(NULL);
    return NULL;
err:
    vTaskDelete(NULL);
    return NULL;
}


static
LIBSSH2_ALLOC_FUNC(libssh2_alloc)
{
    (void) abstract;
    return el_mem_malloc_from_spiram(count);
}

/* libssh2_default_realloc
 */
static
LIBSSH2_REALLOC_FUNC(libssh2_realloc)
{
    (void) abstract;
    return el_mem_realloc_from_spiram(ptr, count);
}


term_ssh_t *term_ssh_create(char *server, char *port, char *user, char *pwd)
{
    int rev;
    struct sockaddr_in sin;
    term_ssh_t *ssh = el_mem_malloc_from_spiram(sizeof(*ssh));

    while (!ssh) {    ESP_LOGE(TAG, "SSH malloc ssh err!"); }
    memset(ssh, 0, sizeof(*ssh));

    ESP_LOGI(TAG, "libssh2_version is %s", LIBSSH2_VERSION);
    rev = libssh2_init(0);
    if(rev) 
    {
        ESP_LOGE(TAG, "libssh2 initialization failed (%d)", rev);
        goto err;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(port));
    sin.sin_addr.s_addr = inet_addr(server);

    ssh->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(ssh->sock == -1) 
    {
        ESP_LOGE(TAG, "Failed to create socket!");
        goto err;
    }

    rev = connect(ssh->sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in));
    if(rev != 0) 
    {
        ESP_LOGE(TAG, "Failed to connect %s.", server);
        goto err;
    }

    ssh->session = libssh2_session_init_ex(libssh2_alloc, NULL, libssh2_realloc, NULL);
    if(!ssh->session)
    {
        ESP_LOGE(TAG, "Failed to session init.");
        goto err;
    }

    ESP_LOGI(TAG, "create ssh:%p session:%p sock:%d\n", ssh, ssh->session, ssh->sock);
    ssh->server = strdup(server);
    ssh->port   = strdup(port);
    ssh->user   = strdup(user);
    ssh->pwd    = strdup(pwd);
    return ssh;
err:
    if (ssh)
    {
        if (ssh->session)
        {
            ssh->session = NULL;
        }
        el_mem_free(ssh);
    }
    return NULL;
}

void term_ssh_set_looper(term_ssh_t *ssh, el_looper_t *looper)
{
    ssh->looper = looper;
}

void term_ssh_set_conn_cb(term_ssh_t *ssh, conn_cb_t cb)
{
    ssh->conn_cb = cb;
}

void term_ssh_set_recv_cb(term_ssh_t *ssh, recv_cb_t cb)
{
    ssh->recv_cb = cb;
}

StackType_t sshStack[4*1024];
StaticTask_t sshTCB;
void term_ssh_connect(term_ssh_t *ssh)
{
    ssh->thread = xTaskCreateStatic(ssh_task, "ssh", 4*1024, 
                    ssh, tskIDLE_PRIORITY + 2, sshStack, &sshTCB);
    //el_thread_create(&ssh->thread, NULL, 0, ssh_task, ssh);
}

void term_ssh_write(term_ssh_t *ssh, char *buffer, int len)
{
    libssh2_channel_write(ssh->channel, buffer, len);
}
