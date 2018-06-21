#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
using namespace std;
#include "min.hpp"

int magicMin(int a, int b) {
	return (a < b) ? a : b;
}

int sequentialReductionArray(int * v, int n) {
    int res = v[0];
    for (int i = 1; i < n; i++) {
        res = magicMin(res, v[i]);
    }
    return res;
}

__global__
void cudaReduceArray(int * in, int * out, int n) {
    extern __shared__ int sdata[]; // Array with size BLOCK_NUM

    int tid = threadIdx.x;
    int i = blockIdx.x * blockDim.x * 2 + threadIdx.x;

    int cur, nxt;
    cur = (i < n) ? in[i] : INF;
    nxt = (i + blockDim.x < n) ? in[i + blockDim.x] : INF;
    
    sdata[tid] = min(cur, nxt);

    __syncthreads();    
    
    for (int s = blockDim.x / 2; s > 0; s /= 2) {
        if (tid < s) {
            sdata[tid] = min(sdata[tid], sdata[tid+s]);
        }
        __syncthreads();
    }

    if (tid == 0) out[blockIdx.x] = sdata[0];
}

int * sequentialReductionMatrix(Matrices m) {
    int * res = (int *)malloc(9 * sizeof(int));
    for (int i = 0; i < 9; i++) {
        res[i] = sequentialReductionArray(m.v[i], m.length);
    }
    return res;
}

int* cudaReduceMatrix(Matrices m) {
    int * res = (int *)malloc(9 * sizeof(int));
    const int THREAD_NUM = 32;
    const int BLOCK_NUM =  (m.length + 2 * THREAD_NUM - 1)/ (2 * THREAD_NUM);

    for (int i = 0; i < 9; i++) {
        int * cuda_block_results;
        int * host_block_results;
        
        cudaMalloc(&cuda_block_results, BLOCK_NUM * sizeof(int));
        errorCheck();

        cudaReduceArray<<<BLOCK_NUM, THREAD_NUM, THREAD_NUM * sizeof(int)>>>(m.dv[i], cuda_block_results, m.length);
        errorCheck();

        host_block_results = (int *)malloc(BLOCK_NUM * sizeof(int));
        cudaMemcpy(host_block_results, cuda_block_results, BLOCK_NUM * sizeof(int), cudaMemcpyDeviceToHost);
        errorCheck();

        res[i] = host_block_results[0];
        for (int k = 1; k < BLOCK_NUM; k++) {
            res[i] = min(res[i], host_block_results[k]);            
        }
        errorCheck();
        
        cudaFree(cuda_block_results);
        free(host_block_results);
    }

    return res;
}
