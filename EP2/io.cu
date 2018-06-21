#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "io.hpp"
#include "error_handler.hpp"

#define debug asm("int $3")

using namespace std;

Matrices readMatricesFromFile(const char *fileName) {
    string line;
    ifstream infile(fileName);
    // The state of the parser.
    // 0: waiting for number of matrices
    // 1: waiting for matrices numbers
    // 2: waiting for asterisks
    // 3: waiting for EOF
    int state = 0;
    int matrixLine = 0;
    long long fileLine = 0;
    // 2d matrices flattened in a vector
    int ** v = (int **)malloc(sizeof(int *) * 9);
    long long index = 0;
    long long numberOfMatrices, currentMatrix = -1;
    bool doContinue = true;
    stringstream err;
    while (getline(infile, line) && doContinue) {
        fileLine++;
        istringstream it(line);
        switch (state) {
            case 0: {
                long long n;
                if (!(it >> n)) {
                    err << "readMatricesFromFile:" << fileLine
                        << ": Expected number";
                    throw runtime_error(err.str());
                }
                else {
                    numberOfMatrices = n;
                    // 9 numbers per matrix
                    for (int i = 0; i < 9; i++)
                        v[i] = (int *)malloc(sizeof(int) * n);;
                    state = 2;
                }
                break;
            }
            case 1: {
                int a, b, c;
                if (!(it >> a >> b >> c)) {
                    err << "readMatricesFromFile:" << fileLine
                    << ": Expected number";
                    throw runtime_error(err.str());
                }
                else {
                    v[(matrixLine) * 3][currentMatrix] = a;
                    v[(matrixLine) * 3 + 1][currentMatrix] = b;
                    v[(matrixLine) * 3 + 2][currentMatrix] = c;

                    // v[(matrixLine * 3) * numberOfMatrices + currentMatrix] = a;
                    // v[(matrixLine * 3 + 1) * numberOfMatrices + currentMatrix] = b;
                    // v[(matrixLine * 3 + 2) * numberOfMatrices + currentMatrix] = c;

                    index += 3;
                    if (matrixLine == 2) {
                        // go to asterisks
                        matrixLine = 0;
                        state = 2;
                    }
                    else {
                        matrixLine++;
                    }
                }
                break;
            }
            case 2: {
                // asterisks
                string ask;
                if (!(it >> ask) || ask.compare("***") != 0) {
                    err << "readMatricesFromFile:" << fileLine
                        << ": Expected '***', got " << ask;
                    throw runtime_error(err.str());
                }
                if (currentMatrix >= numberOfMatrices - 1) {
                    state = 3;
                }
                else {
                    currentMatrix++;
                    state = 1;
                }
                break;
            }
            case 3: {
                // Throws the rest away.
                doContinue = false;
                break;
            }
            default: {
                err << "readMatricesFromFile:" << fileLine
                    << ": invalid parser state: " << state;
                throw runtime_error(err.str());
            }
        }
    }
    if (state != 3) {
        err << "readMatricesFromFile:" << fileLine
            << ": could not read matrices from " << fileName;
        throw runtime_error(err.str());
    }
    
    Matrices result;
    result.v = (int **)malloc(sizeof(int *) * 9);
    result.dv = (int **)malloc(sizeof(int *) * 9);
    for (int i = 0; i < 9; i++) {
        result.v[i] = v[i];
        result.dv[i] = NULL;
    }
    result.length = numberOfMatrices;
    return result;
}

void allocateMatricesToCuda(Matrices& m) {
    for (int i = 0; i < 9; i++) {
        cudaMallocManaged(&(m.dv[i]), m.length * sizeof(int));
        errorCheck();
        cudaMemcpy(m.dv[i], m.v[i], m.length * sizeof(int), cudaMemcpyHostToDevice);
        errorCheck();
    }
}
