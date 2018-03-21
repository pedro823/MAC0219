#include <stdio.h>
#include "frog.h"

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
bool check_bad_state(int* v, int v_size, mutex_t *jump) {
    bool ans = true;
    pthread_mutex_lock(jump);
    for (int i = 0; i < v_size; i++) {
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
