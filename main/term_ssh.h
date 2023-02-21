#ifndef __TERM_SSH_H__
#define __TERM_SSH_H__

#include <el_looper.h>
#include "libssh2_config.h"
#include <libssh2.h>
#include "ring_buf.h"


#define SSH_ERR_OK        0
#define SSH_ERR_HANDSHAKE 1
#define SSH_ERR_PASSWORD  2
#define SSH_ERR_CHANNEL   3
#define SSH_ERR_SHELL     4
#define SSH_ERR_READ      5

extern const char *ssh_err_msg[];

typedef struct term_ssh_t term_ssh_t;

typedef void (* conn_cb_t)(term_ssh_t *ssh, int err);
typedef void (* recv_cb_t)(term_ssh_t *ssh, void *buffer, int len);

typedef struct term_ssh_t
{
    el_looper_t *looper;
    el_event_t *read_event;
    int sock;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    el_thread_t thread;

    ring_buf_s ring_buf;
    char buffer[1024*32];

    char *server;
    char *port;
    char *user;
    char *pwd;

    conn_cb_t conn_cb;
    recv_cb_t recv_cb;
    void *user_data;
} term_ssh_t;


term_ssh_t *term_ssh_create(char *server, char *port, char *user, char *pwd);
void term_ssh_set_looper(term_ssh_t *ssh, el_looper_t *looper);
void term_ssh_set_conn_cb(term_ssh_t *ssh, conn_cb_t cb);
void term_ssh_set_recv_cb(term_ssh_t *ssh, recv_cb_t cb);
void term_ssh_connect(term_ssh_t *ssh);
void term_ssh_write(term_ssh_t *ssh, char *buffer, int len);
void term_ssh_destroy(term_ssh_t *ssh);

#endif