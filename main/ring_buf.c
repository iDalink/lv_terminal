
#include "ring_buf.h"

/*
 * 初始化一个ring buf
 * ring:
 *  要初始化的ring buf的指针
 * buf:
 *  这个ring buf使用的buf
 * size:
 *  这个buf的大小，单位为字节
 * retval:
 * 0: ok
 */
int32_t ring_buf_init(ring_buf_t ring, uint8_t* buf, int32_t size)
{
    ring->vol = size;
    ring->in = 0;
    ring->ou = 0;
    ring->buf = buf;
    return 0;
}

/*
 * 查询一个ring buf中有多少数据，单位为字节
 * retval:
 *  这个ringbuf中的数据量
 */
int32_t ring_buf_size_data(ring_buf_t ring)
{
    int32_t vol, temp;
    temp = ring->in - ring->ou;
    vol = ring->vol;
    if(temp == 0)
    {
        return 0;
    }
    else if(temp > 0)
    {
        return temp;
    }
    else 
    {
        return (vol + temp);
    }
}

/*
 * 为了防止buf被刚好写满，也为了对齐的问题，返回的empty大小比真实的empty少4字节
 * retval:
 *  ring buf中的空的数量，单位为字节
 */
int32_t ring_buf_size_empt(ring_buf_t ring)
{
    int32_t vol, temp;
    temp = ring->ou - ring->in;
    vol = ring->vol;
    if(temp <= 0)
    {
        temp += vol;
    }
    if(temp > 4)
    {
        temp -= 4;
    }
    else
    {
        temp = 0;
    }
    return temp;
}

#include "stdio.h"
/*
 * retval:
 *  实际push的数据量，单位是字节
 */
int32_t ring_buf_push(ring_buf_t ring, uint8_t* buf, int32_t size)
{
    int32_t in, vol, left;
    uint8_t* dptr;
    left = ring_buf_size_empt(ring);
    if(left < size)
    {
		//ring_log("[ring]push warnning size:%d left:%d\n", size, left);
        size = left;
    }
	if(size == 0)
	{
		return 0;
	}
    left = size;
    in = ring->in;
    vol = ring->vol;
    dptr = ring->buf + in; 
    while(left > 0)
    {
        *dptr++ = *buf++;
        left--;
        in++;
        if(in >= vol)
        {
            in = 0;
            dptr = ring->buf;
        }
    }
    ring->in = in;
    return size;
}

/*
 * buf:
 *  如果为NULL,就只消耗ring中的数据
 * retval:
 * 实际pop的数据量
 */
int32_t ring_buf_pop(ring_buf_t ring, uint8_t* buf, int32_t size)
{
    int32_t ou, vol, len;
    uint8_t *sptr;
    len = ring_buf_size_data(ring);
    if(len < size)
    {
        size = len;
    }
    len = size;
    ou = ring->ou;
    vol = ring->vol;
    sptr = ring->buf + ou;
    if(buf == NULL)
    {
        while(len > 0)
        {
            len--;
            ou++;
            if(ou >= vol)
            {
                ou = 0;
            }
        }
        sptr = ring->buf + ou;
    }
    else
    {
        while(len > 0)
        {
            *buf++ = *sptr++;
            len--;
            ou++;
            if(ou >= vol)
            {
                ou = 0;
                sptr = ring->buf;
            }
        }
    }
    ring->ou = ou;
    return size;
}

int32_t ring_buf_push_fifo(ring_buf_t ring, uint8_t* buf, int32_t size)
{
    if(ring_buf_size_empt(ring) <= size)
    {
        ring_buf_pop(ring, NULL, size);
        //printf("^");
    }
    return ring_buf_push(ring, buf, size);
}

