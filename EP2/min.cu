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
    // each thread loads one element from global to shared mem
    int tid = threadIdx.x;
    int n = m.length;
    int start = (tid * n) / 288;
    int end = (tid * (n + 1)) / 288;
    
    for (int s = start; s < end; s++) {
        m.dv[start] = min(m.dv[start], m.dv[end]);
    }    

    __syncthreads();

    if (tid == 0) {
        for (int k = 0; k < 288; k++) {
            m.dv[0] = min(m.dv[0], m.dv[(k * n) / 288] );
        }
    }

    __syncthreads();
}
