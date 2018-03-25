#include <stdio.h>
#include <stdlib.h>
#include "frog.h"

typedef struct simulate_ret {
    int *v;
    int counter;
    int could_jump;
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

void print_vector(int * v, int v_size) {
    printf("\n[");
    for (int i = 0; i < v_size; i++) {
        printf("%d ", v[i]);
    }
    printf("]\n");
}

/*
 * Checks if the program has ended in a
 * bad state.
 */
bool check_deadlock(int* v, int v_size, pthread_mutex_t *jump) {
    bool ans = true;
    pthread_mutex_lock(jump);
    for (int i = 0; i < v_size && ans; i++) {
        if (v[i] == -1) {
            // frog goes left
            if (i > 0 && v[i - 1] == 0) {
                // frog could jump
                printf("frog %d could jump\n", i);
                ans = false;
            }
            if (i > 1 && v[i - 2] == 0) {
                // frog could leap
                printf("frog %d could leap\n", i);
                ans = false;
            }
        }
        else if (v[i] == 1) {
            // frog goes right
            if (i < v_size - 1 && v[i + 1] == 0) {
                // frog could jump
                printf("frog %d could jump\n", i);
                ans = false;
            }
            if (i < v_size - 2 && v[i + 2] == 0) {
                // frog could leap
                printf("frog %d could leap\n", i);
                ans = false;
            }
        }
    }
    print_vector(v, v_size);
    pthread_mutex_unlock(jump);
    return ans;
}

void fill_frog(fargs * frog, int position, bool direction, int * v, int v_size,
        int * counter, int * stop, pthread_barrier_t * barrier, pthread_mutex_t * jump) {
    frog->position = position;
    frog->direction = direction;
    frog->v = v;
    frog->v_size = v_size;
    frog->counter = counter;
    frog->stop = stop;
    frog->barrier = barrier;
    frog->jump = jump;
}

/*
 * Should return: int *v, int COUNTER, time_t elapsed_time
 */
simulate_ret *simulate(int v_size, int LIMIT) {
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
    int * stop;
    simulate_ret * simulation;
    clock_t begin, end;
    begin = clock();

    frog_args = (fargs *)malloc(sizeof(fargs) * v_size);
    vec = (int *)malloc(sizeof(int) * v_size);
    barrier = (pthread_barrier_t *)malloc(sizeof(pthread_barrier_t));
    mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    simulation = (simulate_ret *)malloc(sizeof(simulate_ret));
    threads = (pthread_t *)malloc(sizeof(pthread_t) * v_size);
    COUNTER = (int *)malloc(sizeof(int));
    stop = (int *)malloc(sizeof(int));

    (*COUNTER) = 0;
    (*stop) = 0;

    // initializes mutex
    pthread_mutex_init(mutex, NULL);
    //Initializes frogs
    for (i = 0; i < v_size; i++) {
        if (i < v_size / 2) vec[i] = 1;
        else if (i > v_size / 2) vec[i] = -1;
        else vec[i] = 0;
    }

    for (i = 0; i < v_size; i++) {
        if (i == v_size / 2) continue;
        bool dir = (i < v_size / 2) ? 1 : 0;
        fill_frog(&frog_args[i], i, dir, vec, v_size, COUNTER, stop, barrier, mutex);
    }

    pthread_barrier_init(barrier, NULL, v_size);

    for (int i = 0; i < v_size; i++) {
        if (i == v_size / 2) continue;
        pthread_create(&threads[i], NULL, frog_func, &frog_args[i]);
    }


    pthread_barrier_wait(barrier);


    while ((*COUNTER) <= LIMIT) {
        printf("counter=%d\n", *COUNTER);
    } // waits for counter

    (*stop) = 1;

    // destroys threads
    for (i = 0; i < v_size; i++) {
        if (i == v_size / 2) continue;
        pthread_join(threads[i], NULL);
    }

    if (check_good_state(vec, v_size)) {
        printf("Frogs could finish the challenge\n");
    }

    if (check_deadlock(vec, v_size, mutex)) {
        printf("frogs in deadlock\n");
        simulation->could_jump = 0;
    } else {
        printf("Frogs could still jump\n");
        simulation->could_jump = 1;
    }
    printf("Done simulating final_counter=%d\n", *COUNTER);

    pthread_barrier_destroy(barrier);
    free(mutex);
    free(frog_args);
    free(threads);

    simulation->counter = *COUNTER;
    simulation->v = vec;
    end = clock();
    simulation->elapsed_time = (double) (end - begin) / CLOCKS_PER_SEC;

    return simulation;
}

void free_simulation(simulate_ret * simulation) {
    free(simulation->v);
    free(simulation);
}

int main() {
    int i, counter;
    double sum = 0;
    int vec_size = 5;
    simulate_ret * ret = simulate(vec_size, MAX_COUNTER);
    printf("--> %d %lf\n", ret->counter, ret->elapsed_time);
    free_simulation(ret);
}
