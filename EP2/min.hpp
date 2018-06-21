#pragma once

#include "io.hpp"
#include "error_handler.hpp"

const int INF = INT_MAX;

int abs(int x);

int magicMin(int a, int b);

int sequentialReductionArray(int * v, int n);

__global__
void cudaReduceArray(int * in, int * out);

int * sequentialReductionMatrix(Matrices m);

int * cudaReduceMatrix(Matrices m);
