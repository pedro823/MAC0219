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
    __shared__ int sdata[2 * 288];
    
    int tid = threadIdx.x;
    int globalId = blockIdx.x*blockDim.x + threadIdx.x;
    int n = m.length;
    int start = 2 * blockIdx.x * blockDim.x;

    if ((start + tid) < n) {
        sdata[tid] = m.dv[start + tid];
    }
    else {
        sdata[tid] = 0.0;
    }
    
    if ((start + blockDim.x + tid) < n) {
        sdata[blockDim.x + tid] = m.dv[start + blockDim.x + tid];
    }
    else {
        sdata[blockDim.x + tid] = 0.0;
    }

    // Traverse reduction tree
    for (unsigned int stride = blockDim.x; stride > 0; stride /= 2) {
        __syncthreads();
        if (tid < stride)
            sdata[tid] = min(sdata[tid], sdata[tid + stride]);
    }
    __syncthreads();
    
    // Write the computed sum of the block to the output vector at correct index
    if (tid == 0 && (globalId*2) < n) {
        m.dv[blockIdx.x] = sdata[tid];
    }
}
