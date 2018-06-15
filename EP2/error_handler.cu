#include "error_handler.hpp"
#include <sstream>
#include <stdexcept>

using namespace std;

void cudaErrorHandler(const char *file, int line) {
    cudaThreadSynchronize();
    stringstream err;
    cudaError_t lastError = cudaGetLastError();
    if (lastError != cudaSuccess) {
        err << file << ":" << line <<  ": CudaError: "
            << cudaGetErrorString(lastError) << endl;
        throw runtime_error(err.str());
    }
}
