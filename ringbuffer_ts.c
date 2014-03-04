/***
 * Van Mai Nguyen Thi <vn4720@bard.edu>
 * Oct 2013
 * CMSC328: Embedded Operating Systems
 *
 * Lab 5: A Simple Ring Buffer
 */

#include <string.h>
#include <pthread.h>
#include "ringbuffer.h"

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)

struct ringbuffer{
    int capacity;
    size_t datasize;
    int start;
    int end;
    int items;
    void* data;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
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
        pthread_mutex_init(&rb->mtx, NULL);
        pthread_cond_init(&rb->cond, NULL);
    }
    return rb;
}

void rb_destroy(ringbuffer_t* rb){
    int s;
    
    s = pthread_mutex_lock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_lock");
    
    free(rb->data);
    free(rb);
    
    s = pthread_mutex_unlock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_unlock");
    
    pthread_mutex_destroy(&rb->mtx);
    pthread_cond_destroy(&rb->cond);
}

int rb_put(ringbuffer_t* rb, void* value){
    int s;
    void *place;
    int r = 0;
    
    s = pthread_mutex_lock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_lock");
    
    place = rb->data + ((rb->end*rb->datasize) % (rb->capacity*rb->datasize));
    memcpy(place, value, rb->datasize);
    rb->end = (rb->end + 1) % rb->capacity;
    if (rb->items < rb->capacity) rb->items++;
    else r = -1;
    
    s = pthread_mutex_unlock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_unlock");
    
    s = pthread_cond_signal(&rb->cond);     /* Wake sleeping consumer */
    if (s != 0)
        perror("pthread_cond_signal");
    
    return r;
}

int rb_get(ringbuffer_t* rb, void* ptr){
    int s;
    void* place;
    int r = 0;
    
    s = pthread_mutex_lock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_lock");
    
    while (rb->items == 0) {                /* Wait for sth to consume */
        s = pthread_cond_wait(&rb->cond, &rb->mtx);
        if (s != 0)
            perror("pthread_cond_wait");
    }
    
    if (rb->items > 0){
        place = rb->data + ((rb->start*rb->datasize) % (rb->capacity*rb->datasize));
        memcpy(ptr, place, rb->datasize);
        rb->start = (rb->start + 1) % rb->capacity;
        rb->items--;
    } else{
        r = -1;
    }

    
    s = pthread_mutex_unlock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_unlock");
    
    return r;
}

int rb_numitems(ringbuffer_t* rb){
    int answer, s;
    
    s = pthread_mutex_lock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_lock");
    
    answer = rb->items;
    
    s = pthread_mutex_unlock(&rb->mtx);
    if (s != 0)
        perror("pthread_mutex_unlock");
    
    return answer;
}

int rb_capacity(ringbuffer_t* rb){
     int answer,s;
     
     s = pthread_mutex_lock(&rb->mtx);
     if (s != 0)
     perror("pthread_mutex_lock");
    
     answer = rb->capacity;
     
     s = pthread_mutex_unlock(&rb->mtx);
     if (s != 0)
     perror("pthread_mutex_unlock");
     
     return answer;
}

