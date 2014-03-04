#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "ringbuffer.h"

#define BUF_SIZE 1024
#define DELAY 10

ringbuffer_t* buffer;

typedef struct{
    char msg;
} data_t;

void* consume(void* v){
    data_t d;
    while (1){
        if (rb_numitems(buffer) > 0) {
            rb_get(buffer, &d);        
            printf("*%c\n", d.msg);
        }
    }
    return 0;
}

void *timer(void *v){
    data_t d;
    int i = (int)v;
    while (1){
        usleep(i);
        if (rb_numitems(buffer) < BUF_SIZE){
            d.msg = '+';
            rb_put(buffer, &d); 
        }
    }
    return 0;
}

void* produce(void *v){
    data_t d;
    int ch;    
    while (1){
        ch = getchar();
        if (rb_numitems(buffer) < BUF_SIZE){
            d.msg = ch;
            rb_put(buffer, &d); 
        }
    }
    return 0;
}

int main(int argc, char *arg[]){
    pthread_t put_thread, get_thread, timer_thread;
    buffer = rb_create(BUF_SIZE, sizeof(data_t));
    
    pthread_create(&put_thread, 0, produce, 0);
    pthread_create(&get_thread, 0, consume, 0);
    pthread_create(&timer_thread, 0, timer, (void*)1000000);
    pthread_join(put_thread, 0);
    pthread_join(get_thread, 0);
    pthread_join(timer_thread, 0);

    rb_destroy(buffer);
    return 0;
}
