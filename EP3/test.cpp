#include "io.hpp"
#include <iostream>

using namespace std;

int main() {
    Matrices a = readMatricesFromFile("asd.txt");
    for (long long i = 0; i < a.length; i++) {
        cout << a.v[i] << " ";
    }
    cout << endl;
}
