#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "ringbuffer.h"

#define BUF_SIZE 1000000

ringbuffer_t* buffer;
char *spinner = "\\|/-";

typedef struct{
    int t;
} data_t;

void* consume(void* v){
    data_t d;
    int last = 0;
    while (1){
        if (rb_numitems(buffer) > 0) {
            rb_get(buffer, &d);        
            if (last != 0 && d.t - last != 1){
                printf("*error %d %d\r\n", d.t, last);
            } 
            last = d.t;
        }
    }
    return 0;
}

void* produce(void *v){
    data_t d = {.t = 0};
    while (1){
        if (rb_numitems(buffer) < BUF_SIZE){
            rb_put(buffer, &d); 
            printf("%c\b", spinner[d.t % 4]);                
            fflush(stdout);
            d.t++;            
        }
    }
    return 0;
}

int main(int argc, char *arg[]){
    pthread_t put_thread, get_thread;
    buffer = rb_create(BUF_SIZE, sizeof(data_t));
    
    pthread_create(&put_thread, 0, produce, 0);
    pthread_create(&get_thread, 0, consume, 0);
    pthread_join(put_thread, 0);
    pthread_join(get_thread, 0);

    rb_destroy(buffer);
    return 0;
}
