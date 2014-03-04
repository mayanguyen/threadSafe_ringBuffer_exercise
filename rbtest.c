#include <stdio.h>
#include <stdlib.h>
#include "ringbuffer.h"

#define MAX_OPS   5000
#define BUF_SIZE  500

typedef struct{
    int x;
    int y;
    char c;
} point_t;

int main(int argc, char *arg[]){
    int i, j;
    point_t p;
    ringbuffer_t* buffer = rb_create(BUF_SIZE, sizeof(point_t));
    
    for (i = 1; i < MAX_OPS; i++){

        // some puts        
        for (j = 0; j < rand() % MAX_OPS; j++){     
            p.x = i;
            p.y = j;
            rb_put(buffer, &p);
            printf("+size: %d %d %d\n", rb_numitems(buffer), p.x, p.y);
        }
        
        //some gets
        for (j = 0; j < rand()%MAX_OPS; j++){
            printf("-size: %d ", rb_numitems(buffer));
            if (rb_numitems(buffer) > 0) {
                rb_get(buffer, &p);        
                printf("%d %d", p.x, p.y);
            }
            printf("\n");
        }
    }
    
    rb_destroy(buffer);
    return 0;
}
