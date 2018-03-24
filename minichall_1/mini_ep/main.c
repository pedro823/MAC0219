#include <stdio.h>
#include <stdlib.h>
#include "frog.h"

typedef struct simulate_ret {
    int *v;
    int counter;
    double elapsed_time;
} simulate_ret;

/*
 * Checks if the program has ended in a
 * good state.
 */
bool check_good_state(int* v, int v_size) {
    int i;
    for (i = 0; i < v_size / 2; i++) {
        if (v[i] != -1) {
            return false;
        }
    }
    for (i = (v_size + 3) / 2; i < v_size; i++) {
        if (v[i] != 1) {
            return false;
        }
    }
    return true;
}

/*
 * Checks if the program has ended in a
 * bad state.
 */
bool check_bad_state(int* v, int v_size, pthread_mutex_t *jump) {
    bool ans = true;
    pthread_mutex_lock(jump);
    for (int i = 0; i < v_size && ans; i++) {
        if (v[i] == -1) {
            // frog goes left
            if (i > 0 && v[i - 1] == 0) {
                // frog could jump
                ans = false;
            }
            if (i > 1 && v[i - 2] == 0) {
                // frog could leap
                ans = false;
            }
        }
        else if (v[i] == 1) {
            // frog goes right
            if (i < v_size - 1 && v[v_size + 1] == 0) {
                // frog could jump
                ans = false;
            }
            if (i > v_size - 2 && v[v_size + 2] == 0) {
                // frog could leap
                ans = false;
            }
        }
    }
    pthread_mutex_unlock(jump);
    return ans;
}

void fill_frog(fargs * frog, int position, bool direction, int * v, int v_size,
        int * counter, pthread_barrier_t * barrier, pthread_mutex_t * jump) {
    frog->position = position;
    frog->direction = direction;
    frog->v = v;
    frog->v_size = v_size;
    frog->counter = counter;
    frog->barrier = barrier;
    frog->jump = jump;
}


/*
 * Should return: int *v, int COUNTER, time_t elapsed_time
 */
simulate_ret *simulate(int v_size) {
    if (v_size % 2 == 0) {
        fprintf(stderr, "We can only simulate for odd number of positions");
        exit(EXIT_FAILURE);
    }

    fargs * frog_args;
    int * vec;
    pthread_barrier_t * barrier;
    pthread_t * threads;
    pthread_mutex_t * mutex;
    int i;
    int * COUNTER;
    simulate_ret * simulation;
    clock_t begin, end;
    begin = clock();

    frog_args = (fargs *)malloc(sizeof(fargs) * (v_size - 1));
    vec = (int *)malloc(sizeof(int) * v_size);
    barrier = (pthread_barrier_t *)malloc(sizeof(pthread_barrier_t));
    mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    threads = (pthread_t *)malloc(sizeof(pthread_t) * (v_size - 1));
    simulation = (simulate_ret *)malloc(sizeof(simulate_ret));
    COUNTER = (int *)malloc(sizeof(int));
    (*COUNTER) = 0;
    printf("checkpoint 2\n");

    //Initializes frogs
    for (i = 0; i < v_size; i++) {
        if (i < v_size / 2) vec[i] = 1;
        else if (i >= (v_size + 1) / 2) vec[i] = -1;
        else vec[i] = 0;
    }

    printf("Vector:\n");
    for (i = 0; i < v_size; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");

    for (i = 0; i < v_size; i++) {
        if (i == (v_size + 1) / 2) continue;
        bool dir = (i < v_size / 2) ? 1 : 0;
        fill_frog(&frog_args[i], i, dir, vec, v_size, COUNTER, barrier, mutex);
    }

    pthread_barrier_init(barrier, NULL, v_size);

    for (int i = 0; i < v_size; i++) {
        if (i == (v_size + 1) / 2) continue;
        pthread_create(&threads[i], NULL, frog_func, &frog_args[i]);
    }
    printf("checkpoint 3\n");
    printf("created all threads\n");


    pthread_barrier_wait(barrier);

    printf("Starting simulating\n");

    while ((*COUNTER) <= 100) {
        // printf("SIMULATE=%d\n", *COUNTER);
    }

    printf("Done simulating %d\n", *COUNTER);

    // destroys threads
    for (i = 0; i < v_size; i++) {
        if (i == (v_size + 1) / 2) continue;
        pthread_cancel(threads[i]);
    }

    if (check_good_state(vec, v_size)) {
        printf("Frogs could finish the challenge\n");
    } else if (check_bad_state(vec, v_size, mutex)) {
        printf("Frogs could still jump\n");
    }


    // pthread_barrier_destroy(barrier);
    // free(mutex);
    // free(frog_args);
    // free(threads);

    simulation->counter = *COUNTER;
    simulation->v = vec;
    end = clock();
    // simulation->elapsed_time = (double) (end - begin) / CLOCKS_PER_SEC;
    simulation->elapsed_time = (double) CLOCKS_PER_SEC;
    free(COUNTER);

    printf("Vector:\n");
    for (i = 0; i < v_size; i++) {
        printf("%d ", vec[i]);
    }
    printf("\n");

    return simulation;
}

void free_simulation(simulate_ret * simulation) {
    // free(simulation->v);
    free(simulation);
}

int main() {
    int i;
    double sum = 0;
    int vec_size = 5;
    printf("checkpoint 1\n");
    simulate_ret * ret = simulate(vec_size);
    printf("--> %d %llf\n", ret->counter, ret->elapsed_time);
    free_simulation(ret);
    // for (i = 0; 1 < 10; i++) {
    //     simulate_ret * ret = simulate(vec_size);
    //     printf("--> %d %llf\n", ret->counter, ret->elapsed_time);
    //     int j;
    //     printf("--> ");
    //     for (j = 0; j < vec_size; j++) {
    //         printf("%d ", ret->v[j]);
    //     }
    //     printf("\n");
    //     sum += ret->elapsed_time;
    //     free_simulation(ret);
    // }

    // printf("==> %lf\n", sum);
}
