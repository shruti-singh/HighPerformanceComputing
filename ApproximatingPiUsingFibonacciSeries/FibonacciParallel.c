#include<stdio.h>
#include<omp.h>
int main()
{
	unsigned long n = 40000;
	long k=5;
	long end=n/2+1;
	long steps;	
	unsigned long fib[n];
	fib[0]=1;
	fib[1]=1;
	fib[2]=2;
	fib[3]=3;
	fib[4]=5;
	double time1=omp_get_wtime();
	while(k<=n)
	{
		steps = (k-1)/4;		
		#pragma omp parallel
		{
			int i=omp_get_thread_num();
			int j;
			for(j=i*steps+1;j<=(i+1)*steps;j++)
			{	if((j+k)<=n)
				{
					fib[(j+k)-1]=fib[(j+1)-1]*fib[k-1]+fib[(k-1)-1]*fib[j-1];
					//printf("%d = %ld\n",i,fib[(j+k)-1]);
				}
				else
					break;			
			}		
		}
		k=2*k-1;
	}	
	printf("%ld\n",fib[39]);
	printf("time taken = %lf\n",omp_get_wtime()-time1);
}
