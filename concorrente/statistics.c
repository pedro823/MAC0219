#include "statistics.h"

#include <stdio.h>
#include <math.h>

void statistics_print_array(int n, const int v[])
{
	int i;
	putchar('[');
	for (i = 0; i < n - 1; ++i)
		printf("%d, ", v[i]);
	printf("%d", v[n-1]);
	puts("]");
}

double statistics_average(int n, const int v[])
{
	double sum = 0.;
	int i;
	for (i = 0; i < n; ++i)
		sum += v[i]; /*Ignore overflows*/
	return sum/n;
}

double statistics_stddev(int n, const int v[], const double* avg)
{
	double sum = 0;
	double my_avg;
	int i;

	if (avg != NULL)
		my_avg = *avg;
	else
		my_avg = statistics_average(n, v);

	for (i = 0; i < n; ++i)
		sum += (v[i] - my_avg)*(v[i] - my_avg); /*Ignore overflow*/
	return sqrt(sum/n); /*idk if the sample stddev is better here*/
}

void statistics_print(int n, const int v[])
{
	double avg, stddev;
	avg = statistics_average(n, v);
	stddev = statistics_stddev(n, v, &avg);

	printf("\"access_avg\": %f,\n", avg);
	printf("\"access_stddev\": %f,\n", stddev);
	printf("\"access_array\": ");
	statistics_print_array(n, v);
}
