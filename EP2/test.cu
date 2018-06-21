#include "io.hpp"
#include "min.hpp"
#include "error_handler.hpp"
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>
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
    
    struct timeval begin, end;

    bool same_ans = true;
    int elapsed = 0;

    gettimeofday(&begin, NULL);
    int * cudaAns = cudaReduceMatrix(a);
    cudaDeviceSynchronize();
    gettimeofday(&end, NULL);
        
    elapsed = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
        
    printf("Cuda ans -- %d microseconds\n", elapsed);
    for (int i = 0; i < 9; i++) {
        printf("%d ", cudaAns[i]);
    }

    gettimeofday(&begin, NULL);
    int * seqAns  = sequentialReductionMatrix(a);
    cudaDeviceSynchronize();
    gettimeofday(&end, NULL);
        
    elapsed = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
    
    printf("\n\nSequential ans -- %d microseconds\n", elapsed);
    for (int i = 0; i < 9; i++) {
        printf("%d ", seqAns[i]);
        if (seqAns[i] != cudaAns[i]) same_ans = false;
    }
    printf("\n\n");
    
    printf("%s\n", (same_ans ? "Cuda is CORRECT" : "Cuda is WRONG"));

    return 0;
}
