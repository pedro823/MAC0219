#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int abs(int x) {
	return x & 0x7fffffff;
}

int min(int a, int b) {
	return (a + b - abs(a - b)) >> 1;
}

int main() {
	srand(time(NULL));
	
	for (int k = -900; k < 1000; k++) {
		int i = rand() - (RAND_MAX / 2);
		printf("i = %d, abs(i) = %d\n", i, abs(i));
	}
	return 0;
}
