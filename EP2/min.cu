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
    extern __shared__ int sdata[];
    // each thread loads one element from global to shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < m.length)
        sdata[tid] = m.dv[i];
    __syncthreads();
    
    // do reduction in shared mem
    for (unsigned int s=1; s < blockDim.x; s *= 2) {
        if (tid % (2*s) == 0) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }
    
    // write result for this block to global mem\
    if (blockIdx.x < m.length)
        if (tid == 0) m.dv[blockIdx.x] = sdata[0];
    
}
