#include <pthread.h>
#include <unistd.h>
#include "frog.h"

int valid_position(int position, int v_size) {
    return (position >= 0 && position < v_size);
}

void *frog_func(void *void_frog_args) {
    fargs *frog_args = (fargs *) void_frog_args;
    int position = frog_args->position;
    // jump direction
    int jd = frog_args->direction ? 1 : -1;
    pthread_barrier_t * barrier = frog_args->barrier;
    bool jumped;
    int v_size = frog_args->v_size;
    int * COUNTER = frog_args->counter;

    pthread_barrier_wait(barrier);
    while (1) {
        jumped = 0;

        printf("%d\n", *COUNTER);

        // position 0 or n-1 will never get here
        // if it's moving towards the border
        if (valid_position(position + 1 *jd, v_size) && frog_args->v[position + 1 * jd] == 0) {
            // can jump forward
            if (frog_args->v[position + 1 * jd] == 0) {
                pthread_mutex_lock(frog_args->jump);
                if (frog_args->v[position + 1 * jd] == 0) {
                    frog_args->v[position + 1 * jd] = jd;
                    frog_args->v[position] = 0;
                    position = position + 1 * jd;
                    printf("Jumping %d\n", position);
                    jumped = 1;
                }
                pthread_mutex_unlock(frog_args->jump);
            }
        }
        else {
            // position 1 and n-2 will never get here
            // if it's moving towards the border
            if (valid_position(position + 2 *jd, v_size) && frog_args->v[position + 2 * jd] == 0) {
                // can leap a friend
                if (frog_args->v[position + 2 * jd] == 0) {
                    pthread_mutex_lock(frog_args->jump);
                    if (frog_args->v[position + 2 * jd] == 0) {
                        frog_args->v[position + 2 * jd] = jd;
                        frog_args->v[position] = 0;
                        position = position + 2 * jd;
                        printf("Jumping %d\n", position);
                        jumped = 1;
                    }
                    pthread_mutex_unlock(frog_args->jump);
                }
            }
        }

        if (!jumped) {
            pthread_mutex_lock(&counter_mutex);
            (*COUNTER)++;
            pthread_mutex_unlock(&counter_mutex);
        }
        else {
            pthread_mutex_lock(&counter_mutex);
            (*COUNTER) = 0;
            pthread_mutex_unlock(&counter_mutex);
        }
    }
}
