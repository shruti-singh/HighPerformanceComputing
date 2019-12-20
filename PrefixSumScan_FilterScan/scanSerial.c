//Inclusive Scan

#include<stdio.h>
#include<math.h>
#include<omp.h>

int main(int argc, char **argv){

	long i;
	unsigned long size=atol(argv[1]);
	unsigned long inputArray[size], outputArray[size];

	double timeInit, timeFinal;

	for(i=0;i<size;++i)
		inputArray[i]=(i+1);

	timeInit = omp_get_wtime();
	
	outputArray[0]=0;

	for(i=1;i<size;++i)
	{
		outputArray[i]=outputArray[i-1]+inputArray[i-1];	//outputArray[i-1] contains sum of i-1 elements
	}

	timeFinal=omp_get_wtime();
	// Printing the output array
//	for( i=0;i<size;++i)
//		printf("%d ",outputArray[i]);
	printf("%lf %lf\n",log10((double)size),timeFinal-timeInit);
	return 0;

}
