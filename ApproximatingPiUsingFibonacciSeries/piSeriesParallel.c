#include<stdio.h>
#include<omp.h>
#include<math.h>
int main(int args,char* argv[])
{
	double timeInit, timeFinal;
	double sum=0.0;
	double factor=1.0;
	unsigned long n = atol(argv[1]);
	static long k;
	timeInit = 0.0;
	timeFinal =0.0;
	
	timeInit = omp_get_wtime();	

	#pragma omp parallel for num_threads(4) default(none) reduction(+:sum) private(k,factor) \
		shared(n)
	for(k=0;k<n;k++)
	{
		if(k%2==0)
			factor=1.0;
		else
			factor=-1.0;
		sum+=factor/(2*k+1);
	}
	double pi_approx=4.0*sum;
	timeFinal = omp_get_wtime();
//	printf("%lf\n",pi_approx);
	printf("%lf\n",timeFinal-timeInit);
}
