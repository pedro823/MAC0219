/* 
 * Demo for false sharing variables
 * Created by: Pedro Pereira, 9778794
 */
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define ll long long

int vec_len = 10000;
ll count_up_to = 100000000;
ll v[10000];

void *count_at(void *index_void) {
    int *index = (int *) index_void;
    while (v[*index] < count_up_to) {
        v[*index]++;
    }
    pthread_exit(NULL);
    return NULL;
}

void sequential_count(void) {
    while (v[0] < count_up_to && v[1] < count_up_to) {
        v[0]++;
	v[1]++;
    }
}

void reset_v(void) {
    for (int i = 0; i < vec_len; i++) {
         v[i] = 0;
    }
}

int main(void) {
    clock_t begin, end;
    float elapsed;
    int a, b;
    // declaring threads
    pthread_t ft, st;

    // sequential    
    begin = clock();
    sequential_count();
    end = clock();
    elapsed = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time in sequential: %fs\n", elapsed);

    // with false sharing
    a = 1;
    b = 2;
    begin = clock();
    if (pthread_create(&ft, NULL, count_at, &a)
        || pthread_create(&st, NULL, count_at, &b)) {
        
	fprintf(stderr, "Thread creation error\n");
	return 255;
    }
    if (pthread_join(ft, NULL) || pthread_join(st, NULL)) {
        fprintf(stderr, "Error joining a thread\n");
	return 1;
    }
    end = clock();
    elapsed = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time with false sharing: %fs\n", elapsed);

    // without false sharing
    a = 1;
    b = 999;
    begin = clock();
    if (pthread_create(&ft, NULL, count_at, &a)
        || pthread_create(&st, NULL, count_at, &b)) {
        
	fprintf(stderr, "Thread creation error\n");
	return 255;
    }
    if (pthread_join(ft, NULL) || pthread_join(st, NULL)) {
        fprintf(stderr, "Error joining a thread\n");
	return 1;
    }
    end = clock();
    elapsed = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time without false sharing: %fs\n", elapsed);

    return 0;
}
