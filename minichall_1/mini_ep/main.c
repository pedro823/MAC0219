#include <stdio.h>
#include <stdlib.h>
#include "frog.h"

struct simulate_ret {
    int *v;
    int counter;
    double elapsed_time;
};

struct simulate_mean_ret {
    // amount of deadlocks the simulations ended in.
    int amount_deadlocks;
    // amount of good states the simulations ended in.
    int amount_good_states;
    // amount of times the counter has been >= MAX_COUNTER
    int amount_bursted_counter;
    // the mean of the counters.
    double counter_mean;
    // mean of the elapsed time.
    double elapsed_time_mean;
};

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
 * deadlock state.
 */
bool check_deadlock(int* v, int v_size, mutex_t *jump) {
    bool ans = true;
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
                // frog could leapeu não sei porque ele 
                ans = false;
            }
        }
    }
    return ans;
}

// Should return: int *v, int COUNTER, time_t elapsed_time
// AFTER THE SIMULATION IS STOPPED, THIS FUNCTION SHOULD DESTROY ALL
// CREATED THREADS.
struct simulate_ret simulate(int v_size) {
// tirei o mutex jump da check_deadlock (antiga check_bad_state)
// trave o lock antes de chamar a função e destrave depois,
// fica mais limpo e precisa de menos informação pra função

}

// wrapper for simulate(), that calculates the mean of
// the values provided by it. Simulates `times` times.
struct simulate_mean_ret simulate_many(int v_size, int times) {
    int amount_burst = 0, amount_deadlocks = 0, amount_good_states = 0;
    double counter_mean = 0, elapsed_time_mean = 0;
    struct simulate_ret ret;
    bool was_good_state;
    for (int i = 0; i < times; i++) {
        was_good_state = false;
        ret = simulate(v_size);
        if (check_good_state(ret.v, v_size)) {
            amount_good_states++;
            was_good_state = true;
        }
        if (check_deadlock(ret.v, v_size)) {
            amount_deadlocks++;
        }
        free(ret.v);
        if (ret.counter >= MAX_COUNTER && !was_good_state) {
            amount_burst++;
        }
        counter_mean += (double) ret.counter / times;
        elapsed_time_mean += (double) ret.elapsed_time / times;
    }
    struct simulate_mean_ret result = {
        amount_good_states,
        amount_deadlocks,
        amount_burst,
        counter_mean,
        elapsed_time_mean
    };
    return result;
}

// binary searches for the optimum COUNTER.
// tolerance is an int > 0 which corresponds to
// how far apart the low and high bounds of the
// binary search can be.
// verbose is a bool that, if set to true, allows prints from
// the function.
int bsearch_counter(int v_size, int tolerance, bool verbose) {
    if (tolerance < 0) {
        return -1;
    }
    struct simulate_mean_ret ret;
    int lo_bound = 0, hi_bound = 1e6;
    while (abs(hi_bound - lo_bound) > tolerance) {
        // picks middle
        int cur_counter = lo_bound + hi_bound / 2;
        MAX_COUNTER = cur_counter;
        if (verbose) {
            printf("Simulating frogs with MAX_COUNTER = %d\n", MAX_COUNTER);
        }
        // simulates 20 times
        ret = simulate_many(v_size, 20);
        if (verbose) {
            printf("Mean elapsed time in simulation = %llf\n"
                   "final counter mean = %llf\n",
                   ret.elapsed_time_mean, ret.counter_mean);
        }
        if (ret.amount_bursted_counter > ret.amount_deadlocks) {
            // count bursted before being in a deadlock

        }
    }
    return lo_bound + hi_bound / 2;
}
