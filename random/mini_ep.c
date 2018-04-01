/* 
 * Demo for false sharing variables
 * Created by: Pedro Pereira, 9778794
 * And Breno Moura, 9790973
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
    while (v[0] < count_up_to && v[1] < count_up_to
        && v[2] < count_up_to && v[3] < count_up_to) {
        v[0]++;
    	v[1]++;
        v[2]++;
        v[3]++;
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
    int a, b, c, d;
    // declaring threads
    pthread_t first_thread, second_thread, third_thread, fourth_thread;

    // sequential    
    begin = clock();
    sequential_count();
    end = clock();
    elapsed = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time in sequential: %fs\n", elapsed);
    reset_v();
    
    // with false sharing
    a = 0;
    b = 1;
    c = 2;
    d = 3;
    begin = clock();
    if (pthread_create(&first_thread, NULL, count_at, &a)
        || pthread_create(&second_thread, NULL, count_at, &b)
        || pthread_create(&third_thread, NULL, count_at, &c)
        || pthread_create(&fourth_thread, NULL, count_at, &d)) {
        
	fprintf(stderr, "Thread creation error\n");
	return 255;
    }
    if (pthread_join(first_thread, NULL) 
        || pthread_join(second_thread, NULL)
        || pthread_join(third_thread, NULL)
        || pthread_join(fourth_thread, NULL)) {
        fprintf(stderr, "Error joining a thread\n");
	return 1;
    }
    end = clock();
    elapsed = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time with false sharing: %fs\n", elapsed);
    reset_v();
    
    // without false sharing
    a = 0;
    b = 250;
    c = 500;
    d = 999;
    begin = clock();
    if (pthread_create(&first_thread, NULL, count_at, &a)
        || pthread_create(&second_thread, NULL, count_at, &b)
        || pthread_create(&third_thread, NULL, count_at, &c)
        || pthread_create(&fourth_thread, NULL, count_at, &d)) {
        
	fprintf(stderr, "Thread creation error\n");
	return 255;
    }
    if (pthread_join(first_thread, NULL) 
        || pthread_join(second_thread, NULL)
        || pthread_join(third_thread, NULL)
        || pthread_join(fourth_thread, NULL)) {
        fprintf(stderr, "Error joining a thread\n");
	return 1;
    }
    end = clock();
    elapsed = (float) (end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed time without false sharing: %fs\n", elapsed);
    reset_v();
    
    return 0;
}
