#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>

// Implementation of filter using scan 

int main(int argc, char *argv[]){

	long size=atol(argv[1]);
	long input[size], output[size];
	long aux[size];
	long i,point=0;
	
	for(i=0;i<size;i++)
		input[i]=i+1;
	omp_set_num_threads(4);
	long filter=6;
	double initTime,finalTime;
	initTime=omp_get_wtime();
	#pragma omp parallel for shared(i) schedule(guided)
	for(i=0;i<size;++i)	
	{	if(input[i]>=filter)
		{
			aux[i]=input[i];
		}
		else
		{
			aux[i]=0;
		}	
	}
	
	long t,k;
	
	double end = log10((double)size)/log10(2);
	long d=1, d1;
	for(i=0;i<end;i++)
	{
		d=d<<1;
		d1=d>>1;
		#pragma omp parallel for schedule(dynamic,1) private(k)
		for(k=0;k<size;k=k+d)
		{
			aux[k+d-1]=aux[k+d1-1]+aux[k+d-1];
		}
	}	
		
	aux[size-1]=0;
	d=1;
	for(i=end-1;i>=0;i--)
	{
		d=d<<1;
		d1=d>>1;		
		#pragma omp parallel for  private(k)
		for(k=0;k<size;k=k+d)
		{
			
			t=aux[k+d1-1];
			aux[k+d1-1]=aux[k+d-1];
			aux[k+d-1]=t+aux[k+d-1];
			
					
		}
	}	

	
	long counter=0;
	for(i=0;i<(size-1);++i)
		if(aux[i]<aux[i+1])
			output[counter++]=input[i+1];
	
	finalTime=omp_get_wtime();
	printf("%lf %lf\n",log10(size),finalTime-initTime);
	return 0;
}


