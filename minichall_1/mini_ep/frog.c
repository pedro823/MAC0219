#include <pthread.h>
#include <unistd.h>
#include "frog.h"

int valid_position(int position, int v_size) {
    return (position >= 0 && position < v_size);
}

void printvector(int * v, int v_size) {
    printf("printvector: size %d\n[", v_size);
    for (int i = 0; i < v_size; i++) {
        printf("%d ", v[i]);
    }
    printf("]\n");
}

void *frog_func(void *void_frog_args) {
    fargs * frog_args = (fargs *) void_frog_args;
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

        if (!valid_position(position, frog_args->v_size)) {
            printf("I AM NOT IN A VALID POSITION: %d\n", position);
            break;
        }
        if (frog_args->v[position] != jd) {
            printf("I am in position %d, i should be %d but i am %d\n", position, jd, frog_args->v[position]);
            break;
        }
        // printf("COUNTER=%d\n", *COUNTER);

        // position 0 or n-1 will never get here
        // if it's moving towards the border
        if (valid_position(position + 1 * jd, v_size) && frog_args->v[position + 1 * jd] == 0) {
            // can jump forward
            printf("ENTERED IF11\n");
            if (frog_args->v[position + 1 * jd] == 0) {
                pthread_mutex_lock(frog_args->jump);
                if (frog_args->v[position + 1 * jd] == 0) {
                    printf("ENTERED IF12\n");
                    frog_args->v[position + 1 * jd] = jd;
                    frog_args->v[position] = 0;
                    position = position + 1 * jd;
                    // printf("Jumping %d\n", position);
                    jumped = 1;
                }
                printvector(frog_args->v, frog_args->v_size);
                pthread_mutex_unlock(frog_args->jump);
            }
        }
        else {
            // position 1 and n-2 will never get here
            // if it's moving towards the border
            if (valid_position(position + 2 * jd, v_size) && frog_args->v[position + 2 * jd] == 0) {
                // can leap a friend
                printf("ENTERED IF21\n");
                if (frog_args->v[position + 2 * jd] == 0) {
                    pthread_mutex_lock(frog_args->jump);
                    if (frog_args->v[position + 2 * jd] == 0) {
                        printf("ENTERED IF22\n");
                        frog_args->v[position + 2 * jd] = jd;
                        frog_args->v[position] = 0;
                        position = position + 2 * jd;
                        // printf("Jumping %d\n", position);
                        jumped = 1;
                    }
                    printvector(frog_args->v, frog_args->v_size);
                    pthread_mutex_unlock(frog_args->jump);
                }
            }
        }

        if (!jumped) {
            pthread_mutex_lock(&counter_mutex);
            (*COUNTER)++;
            printvector(frog_args->v, frog_args->v_size);
            pthread_mutex_unlock(&counter_mutex);
        }
        else {
            pthread_mutex_lock(&counter_mutex);
            (*COUNTER) = 0;
            printvector(frog_args->v, frog_args->v_size);
            pthread_mutex_unlock(&counter_mutex);
        }
    }
    return NULL;
}
