#include "io.hpp"
#include "min.hpp"
#include "error_handler.hpp"
#include <iostream>
#include <ctime>
#include <unistd.h>
using namespace std;

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments");
        return 1;
    }

    printf("Reading Matrices\n");

    char * matrixFile = argv[1];
    Matrices a = readMatricesFromFile(matrixFile);
     
    allocateMatricesToCuda(a);

    printf("Matrixes in memory\n");

    printf("there are %d matrices\n", a.length);
    
    clock_t begin, end;
    bool same_ans = true;
    float elapsed = 0;

    begin = clock();
    int * cudaAns = cudaReduceMatrix(a);
    cudaDeviceSynchronize();
    end = clock();

    elapsed = (end - begin) / CLOCKS_PER_SEC;
        
    printf("Cuda ans -- %.10f Seconds\n", elapsed);
    for (int i = 0; i < 9; i++) {
        printf("%d ", cudaAns[i]);
    }

    begin = clock();
    int * seqAns  = sequentialReductionMatrix(a);
    cudaDeviceSynchronize();
    end = clock();

    elapsed = (end - begin) / CLOCKS_PER_SEC;
    
    printf("\n\nSequential ans -- %.10f Seconds\n", elapsed);
    for (int i = 0; i < 9; i++) {
        printf("%d ", seqAns[i]);
        if (seqAns[i] != cudaAns[i]) same_ans = false;
    }
    printf("\n\n");
    
    printf("%s\n", (same_ans ? "Cuda is CORRECT" : "Cuda is WRONG"));

    return 0;
}
