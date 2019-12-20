#include<stdio.h>
#include<omp.h>

double step;
static long num_steps = 1000000000;

int main(){

	double timei = omp_get_wtime();
	omp_set_num_threads(1);
	int i;
	double x,sum=0.0;
	double pi;
	step = 1.0/(double)num_steps;
	for(i=0;i<num_steps;i++)
	{
		x = (i+0.5)*step;
		sum = sum+4.0/(1.0 + x*x);
	}
	pi = step * sum;
	printf("%.10lf = pi , step = %.10lf\n", pi, step);
	double timef = omp_get_wtime();
	printf("Time taken = %lf \n", timef - timei);

	return 0;	
}
