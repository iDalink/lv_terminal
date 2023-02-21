#ifndef __ring_buf_h__
#define __ring_buf_h__

#include "stdint.h"

/*
 * ou == in的时候ring buf是空的
 * 每次读数据的时候直接读ou的数据，读完后ou++
 * 写的时候直接往in处写数据，写完后in++
 */
typedef struct ring_buf_struct
{
    int32_t vol;
    int32_t in;
    int32_t ou;
    uint8_t *buf;
}ring_buf_s, *ring_buf_t;


int32_t ring_buf_init(ring_buf_t ring, uint8_t* buf, int32_t size);
int32_t ring_buf_size_data(ring_buf_t ring);
int32_t ring_buf_size_empt(ring_buf_t ring);
int32_t ring_buf_push(ring_buf_t ring, uint8_t* buf, int32_t size);
int32_t ring_buf_pop(ring_buf_t ring, uint8_t* buf, int32_t size);
int32_t ring_buf_push_fifo(ring_buf_t ring, uint8_t* buf, int32_t size);


#endif

