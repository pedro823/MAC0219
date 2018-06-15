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
             cout << "Min between " << m.v[start] << " " << m.v[i] << " --> ";
             cout << "Is " << magicMin(m.v[start], m.v[i]) << '\n';
             m.v[start] = magicMin(m.v[start], m.v[i]);
         }
    }
}

__global__
void cudaReduction(Matrices m) {
    //unsigned int tid = threadIdx.x;

    for (int start = 0; start < 9; start++) {
        for (int i = start; i < m.length; i += 9) {
            m.v[start] = min(m.v[start], m.v[i]);
            printf("JOVEM Min between %d %d --> %d\n", m.v[start], m.v[i], min(m.v[start], m.v[i]));
        }
    }

}
