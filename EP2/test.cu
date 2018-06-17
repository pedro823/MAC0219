#include "io.hpp"
#include "min.hpp"
#include "error_handler.hpp"
#include <iostream>

using namespace std;

int main() {
    Matrices a = readMatricesFromFile("asd.txt");
    
//    for (int k = 0; k < a.length; k += 9) {
//        for (int i = 0; i < 9; i++) {
//            if (i % 3 == 0) cout << '\n';
//            cout << a.v[k + i] << " ";
//        }
//        cout << endl;
//    }
//    
//    sequentialReduction(a);
//
// 
//    cout << "RESULT: \n\n";
//    for (int i = 0; i < 9; i++) {
//        if (i % 3 == 0) cout << '\n';
//        cout << a.v[i] << " ";
//    }
//    cout << endl;

//    delete[] a.v;

  
    a = readMatricesFromFile("asd.txt");
    
    for (int i = 0; i < a.length; i++) {
        cout << a.v[i] << " ";
    }
    cout << endl;
    
    allocateMatricesToCuda(&a);

    cout << endl;

    
    cudaReduction<<<1, 288>>>(a);
    errorCheck();

    cout << "ALO --> " << a.dv[0] << '\n';
    cout << endl;

    return 0;
}
