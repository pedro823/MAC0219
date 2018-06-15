#pragma once

/*--
    API for input and output from files and devices.
  --*/

typedef struct {
    int *v;           // Matrices vector, contains flattened matrices
    int *dv;          // Matrices vector inside CUDA device
    long long length; // the length of all the matrices combined.
} Matrices;

/*--
    Reads matrices from file.
    Throws:
        runtime_error: If file was parsed incorrectly.
  --*/
Matrices readMatricesFromFile(const char *fileName);

/*--
    transfers allocated vector to CUDA shared memory.
    DANGER: Frees original memory.
    Throws:
        runtime_error: If matrix could not be allocated.
  --*/
void allocateMatricesToCuda(Matrices m);
