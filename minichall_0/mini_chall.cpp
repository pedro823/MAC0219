#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int main() {
	srand(time(NULL)); // not cryptographically secure

	// generates random false/true vector
	vector<bool> v(10000000);
	for (int i = 0; i < 10000000; i++) {
		v[i] = rand() % 2;
	}

	// This will run faster with sorting before
	clock_t start_unordered = clock();
	int number_of_trues_unordered = 0;
	for (int i = 0; i < 10000000; i++) {
		if (v[i]) {
			number_of_trues_unordered++;
		}
	}
	double elapsed_unordered = static_cast<double> (clock() - start_unordered) / CLOCKS_PER_SEC;

	/*
	 * WITH SORTING
	 */
	sort(v.begin(), v.end());

	clock_t start_ordered = clock();

	int number_of_trues_ordered = 0;
	for (int i = 0; i < 10000000; i++) {
		if (v[i]) {
			number_of_trues_ordered++;
		}
	}
	double elapsed_ordered = static_cast<double> (clock() - start_ordered) / CLOCKS_PER_SEC;

	cout << number_of_trues_ordered << " Should equal " << number_of_trues_unordered << endl;
	cout << "Elapsed time UNORDERED: " << elapsed_unordered << endl;
	cout << "Elapsed time ORDERED: " << elapsed_ordered << endl;
	return 0;
}
