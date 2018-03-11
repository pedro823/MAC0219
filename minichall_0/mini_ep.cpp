#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

long long m = 1000003;

int main() {
	int v[m];
	for (long long i = 0; i < m; i++) {
		v[i] = 1;
	}
	clock_t start_linear = clock();
	int a = 0;
	for (long long i = 1; i != 0; i = (i + 1) % m) {
		a = (a + v[i]) % 255;
	}
	clock_t end_linear = clock(), start_non_linear = clock();
	for (long long i = 12347; i != 0; i = (i + 12347) % m) {
		a = (a + v[i]) % 255;
	}
	clock_t end_non_linear = clock();
	clock_t elapsed_linear = end_linear - start_linear;
	clock_t elapsed_non_linear = end_non_linear - start_non_linear;
	cout << "Elapsed time (ns) in linear: " << elapsed_linear << endl;
	cout << "Elapsed time (ns) non linear: " << elapsed_non_linear << endl;
	return 0;
}
