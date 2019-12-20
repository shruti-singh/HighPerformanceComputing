#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>
int main(int argc, char* argv[])
{
	long size=atol(argv[1]);
	long input[size],output[size];
	long i,point=0;
	for(i=0;i<size;i++)
		input[i]=i+1;
	long filter=size/2;
	double initTime,finalTime;
	initTime=omp_get_wtime();
	for(i=0;i<size;i++)
	{
		if(input[i]>=filter)
		{
			output[point]=input[i];
			point++;	
		}
	}
	finalTime=omp_get_wtime();
	printf("%lf %lf\n",log10(size),finalTime-initTime);	
}
