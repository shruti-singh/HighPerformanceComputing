#include<stdio.h>
#include<omp.h>
int main()
{
	static unsigned long n=40000;
	long num1=0;
	long num2=1;
	long num3;
	unsigned long i;
	double time=omp_get_wtime();
	for(i=1;i<n;i++)
	{
		num3=num1+num2;
		num1=num2;
		num2=num3;
	}
	printf("%ld\n",num3);
	printf("%lf\n",omp_get_wtime()-time);
}
