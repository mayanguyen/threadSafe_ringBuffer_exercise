/***
 * Keith O'Hara <kohara@bard.edu>
 * Oct 2013
 * CMSC328: Embedded Operating Systems
 * 
 * A Simple Ring Buffer
 */

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stdlib.h>

typedef struct ringbuffer ringbuffer_t;

/*
 * Create a ring buffer with capacity number of slots;
 * each slot is datasize big;
 * return 0 on error
 */
ringbuffer_t* rb_create(int capacity, size_t datasize);

/*
 * Dispose of the ring buffer
 */
void rb_destroy(ringbuffer_t* rb);

/* 
 * Add element value to ring buffer
 * returns 0 on success; -1 on overflow
 */
int rb_put(ringbuffer_t* rb, void* value);

/* 
 * Consume an element from the ring buffer
 * returns 0 on success; -1 on underflow
 */
int rb_get(ringbuffer_t* rb, void *ptr);

/*
 * Return the number of elements in the ring buffer
 */
int rb_numitems(ringbuffer_t* rb);

/* 
 * Return the total capacity of the ring buffer
 */
int  rb_capacity(ringbuffer_t* rb);


#endif /* __RING_BUFFER_H__*/
