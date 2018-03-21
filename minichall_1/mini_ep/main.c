#include <stdio.h>
#include "frog.h"

/*
 * Checks if the program has ended in as
 * good state.
 */
bool check_good_state(int* v, int v_size) {
    int i;
    for (i = 0; i < v_size / 2; i++) {
        if (v[i] != -1) {
            return false;
        }
    }
    for (i = (v_size + 3) / 2;)

}
