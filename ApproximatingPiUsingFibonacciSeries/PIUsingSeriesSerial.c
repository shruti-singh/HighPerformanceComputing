#include<stdio.h>
#include<omp.h>
#include<math.h>
int main(int args,char* argv[])
{
	unsigned long n = atol(argv[1]);
	double factor=1.0;
	double sum=0.0;
	double pi_approx;
	int k;
	double time1=omp_get_wtime();
	for(k=0;k<n;k++)
	{
		sum+=factor/(2*k+1);
		factor=-factor;
	}
	pi_approx=4.0*sum;
//	printf("pi = %.10lf\n",pi_approx);
	printf("%lf ",omp_get_wtime()-time1);	
	return 0;
}
