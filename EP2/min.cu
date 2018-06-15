#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
using namespace std;
#include "min.hpp"
 
int magicMin(int a, int b) {
	return (a + b - abs(a - b)) >> 1;
}

void sequentialReduction(Matrices m) {
    for (int start = 0; start < 9; start++) {
         for (int i = start; i < m.length; i += 9) {
            m.v[start] = magicMin(m.v[start], m.v[i]);
         }
    }
}

__global__
void cudaReduction(Matrices m) {
    //unsigned int tid = threadIdx.x;

    for (int start = 0; start < 9; start++) {
        for (int i = start; i < m.length; i += 9) {
            m.dv[start] = min(m.dv[start], m.dv[i]);
            printf("JOVEM Min between %d %d --> %d\n", m.dv[start], m.dv[i], min(m.dv[start], m.dv[i]));
        }
    }

    printf("RESULT: \n\n");
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) printf("\n");
        printf("%d ", m.dv[i]);
    }

    printf("\n");
}
