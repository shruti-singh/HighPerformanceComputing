#include<stdio.h>
#include<omp.h>
#include<math.h>

int main(int argc, char *argv[]){

	long size=atol(argv[1]);
	long input[size],output[size];
	long i,point=0;
	for(i=0;i<size;i++)
		input[i]=i+1;
	long filter=6;
	double initTime,finalTime;
	initTime=omp_get_wtime();
	long counter=0;

	#pragma omp parallel for schedule(dynamic, 1)
	for(i=0;i<size;++i){
		if(input[i]>=filter){
			output[i]=input[i];
		}
		else
			output[i]=0;
	}

	long k;
	for(k=0;k<size;++k)	
		if(output[k]!=0)
			output[counter++]=output[k];
	size=counter;	
//	for(k=0;k<size;k++)
//		printf("%ld ", output[k]);
	
	finalTime=omp_get_wtime();
	printf("%lf %lf\n",log10(size),finalTime-initTime);	
}
