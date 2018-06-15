#pragma once

#include "io.hpp"

int abs(int x);

int magicMin(int a, int b);

void sequentialReduction(Matrices m);

__global__
void cudaReduction(Matrices m);
