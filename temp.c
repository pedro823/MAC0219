
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define IF if (v[i] > max)

#define NUM_EXEC 5

static void populate_vector(size_t N, int v[])
{
    size_t i;
    srand(1337);

    for (i = 0; i < N; ++i)
        v[i] = rand();
}

static double contention_test(size_t N, int T, const int v[])
{
    size_t i;
    int max = -1;
    double t0, t1;

    omp_lock_t mutex;

    omp_init_lock(&mutex);

    t0 = omp_get_wtime();
    #pragma omp parallel for private(i) num_threads(T)
    for (i = 0; i < N; ++i)
    {
        /*--*/
IF IF IF IF IF IF IF IF IF 
        /*--*/
        {
            omp_set_lock(&mutex);
            {
                if (v[i] > max)
                    max = v[i];
            }
            omp_unset_lock(&mutex);
        }
    }
    t1 = omp_get_wtime();

    return (t1-t0);
}

static double avg(int n, const double v[])
{
    int i;
    double sum = 0.;
    for (i = 0; i < n; ++i)
        sum += v[i];
    return sum/n;
}

int main(int argc, char* argv[])
{
    static double times[NUM_EXEC];
    int* vector = NULL;

    size_t N;
    int T, i;

    if (argc != 3)
    {
        fprintf(stdout, "Usage: %s <vector_size> <number_of_threads>\n ", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    T = atoi(argv[2]);
    
    vector = (int*) malloc(N*sizeof(int));

    if (!vector)
    {
        fprintf(stdout, "Failed to allocate memory. Exiting...\n");
        return 2;
    }

    populate_vector(N, vector);

    /*throw away first execution*/
    times[0] = contention_test(N, T, vector);
    for (i = 0; i < NUM_EXEC; ++i)
        times[i] = contention_test(N, T, vector);
    
    fprintf(stdout, " Average of %d executions: %lf s\n", NUM_EXEC, avg(NUM_EXEC, times));
    
    free(vector);
    return 0;    
}

