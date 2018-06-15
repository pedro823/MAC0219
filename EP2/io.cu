#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "io.hpp"

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
    int *v;
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
                    v = new int[9 * n];
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
                    v[index] = a;
                    v[index + 1] = b;
                    v[index + 2] = c;
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
    result.v = v;
    result.length = 9 * numberOfMatrices;
    return result;
}

void allocateMatricesToCuda(Matrices m) {
    int *otherV;
    cudaMallocManaged(&otherV, m.length * sizeof(int));
    for (long long i = 0; i < m.length; i++) {
        otherV[i] = m.v[i];
    }
    delete[] m.v;
    m.v = otherV;
}
