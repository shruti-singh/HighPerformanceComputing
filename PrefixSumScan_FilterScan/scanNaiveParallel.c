//Incluive Scan

#include<stdio.h>
#include<math.h>
#include<omp.h>

int main(int argc, char **argv){
	unsigned long i, j;
	unsigned long size=atol(argv[1]);
	unsigned long inputArray[size], outputArray[size];

	double timeInit, timeFinal;
	//printf("Enter the input elements");
	for(i=0;i<size;++i)
	{	inputArray[i]=i+1;	outputArray[i]=0;}
	/*printf("Input Array:\n");	
	for( i=0;i<size;++i)
		printf("%d ",inputArray[i]);
	printf("\n");*/	

	timeInit = omp_get_wtime();
	
	outputArray[0]=0;
	#pragma omp parallel for private(j)
		for(i=1;i<size;++i)
		{
			for(j=0;j<i;++j)
				outputArray[i]+=inputArray[j];
		}

	timeFinal=omp_get_wtime();
	// Printing the output array
	//int k;
	//printf("Output Array:\n");
	//for( k=0;k<size;++k)
	//	printf("%d ",outputArray[k]);
	printf("%lf %lf\n",log10((double)size),timeFinal-timeInit);
	return 0;

}
