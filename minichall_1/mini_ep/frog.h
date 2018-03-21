#pragma once

#include <pthread.h>

#ifndef bool
#define bool char
#define true 1
#define false 0
#endif

// Global
mutex_t counter_mutex;
int COUNTER = 0;
int MAX_COUNTER = 10;

struct fargs {
    int position; // Frogs' starting position
    bool direction; // 0 == goes left, 1 == goes right
    int *v;
    int v_size;
    mutex_t jump; // Global mutex jump
};

/*
 * initializes frog.
 * void_frog_args is a void* to a struct fargs.
 */
void *frog(void *void_frog_args);
