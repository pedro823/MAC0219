#ifndef FROG_H
#define FROG_H

#include <pthread.h>
#include <unistd.h>

#ifndef bool
#define bool char
#define true 1
#define false 0
#endif

// Global
static pthread_mutex_t counter_mutex;
static int COUNTER = 0;
static int MAX_COUNTER = 100;

typedef struct fargs {
    int position; // Frogs' starting position
    bool direction; // 0 == goes left, 1 == goes right
    int *v;
    int v_size;
    pthread_barrier_t * barrier;
    pthread_mutex_t *jump; // Global mutex jump
} fargs;

/*
 * initializes frog.
 * void_frog_args is a void* to a struct fargs.
 */
void *frog_func(void *void_frog_args);


#endif
