#include "io.hpp"
#include "min.hpp"
#include <iostream>

using namespace std;

int main() {
    Matrices a = readMatricesFromFile("asd.txt");
    
    for (int k = 0; k < a.length; k += 9) {
        for (int i = 0; i < 9; i++) {
            if (i % 3 == 0) cout << '\n';
            cout << a.v[k + i] << " ";
        }
    }
    
    sequentialReduction(a);

    cout << "RESULT: \n\n";
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) cout << '\n';
        cout << a.v[i] << " ";
    }

    delete[] a.v;
    
    a = readMatricesFromFile("asd.txt");
    allocateMatricesToCuda(a);

    for (int i = 0; i < a.length; i++) {
        cout << a.v[i] << ' ';
    }
    cout << endl;

    
    cudaReduction<<<1, 1>>>(a);

    cout << "ALO --> " << a.v[0] << '\n';
    cout << endl;

}
