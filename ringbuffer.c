/***
 * Keith O'Hara <kohara@bard.edu>
 * Oct 2013
 * CMSC328: Embedded Operating Systems
 * 
 * A Simple Ring Buffer
 */

#include <string.h>
#include "ringbuffer.h"

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)

struct ringbuffer{
    int capacity;
    size_t datasize;
    int start;
    int end;
    int items;
    void* data;
};

ringbuffer_t* rb_create(int capacity, size_t datasize){
    ringbuffer_t* rb = malloc(sizeof(ringbuffer_t));
    datasize = align4(datasize);
    if (rb > 0) rb->data = malloc(capacity * datasize);    
    if (rb > 0 && rb->data > 0){
        rb->capacity = capacity;
        rb->datasize = datasize;
        rb->start = 0;
        rb->end = 0;        
        rb->items = 0;
    }
    return rb;
}

void rb_destroy(ringbuffer_t* rb){
    free(rb->data);
    free(rb);
}

int rb_put(ringbuffer_t* rb, void* value){
    void *place;
    int r = 0 ;
    place = rb->data + 
        ((rb->end*rb->datasize) % (rb->capacity*rb->datasize));
    memcpy(place, value, rb->datasize);
    rb->end = (rb->end + 1) % rb->capacity;
    if (rb->items < rb->capacity) rb->items++;
    else r = -1;

    return r;
}

int rb_get(ringbuffer_t* rb, void* ptr){
    void* place;
    int r = 0;
    if (rb->items > 0){
        place = rb->data + 
            ((rb->start*rb->datasize) % (rb->capacity*rb->datasize));
        memcpy(ptr, place, rb->datasize);
        rb->start = (rb->start + 1) % rb->capacity;   
        rb->items--;
    } else{
        r = -1;
    }
    return r;
}

int rb_numitems(ringbuffer_t* rb){
    return rb->items;
}

int  rb_capacity(ringbuffer_t* rb){
    return rb->capacity;
}

