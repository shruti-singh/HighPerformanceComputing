#include<stdio.h>
#include<omp.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
	unsigned long size=atol(argv[1]);
	double timei = omp_get_wtime();
	int toSort[size];
	int i;
	for(i=size; i>0;i--)
	{
		toSort[size-i] = i;
	}
	bubbleSort(toSort,size);
	double timef = omp_get_wtime();
	printf("%lf %lf\n ",log10((double)size), timef - timei);
}
void bubbleSort(int *toSort,unsigned long size)
{
	int i,j,temp;
	for(i = 0;i<size-1;i++)
	{
		for(j=0;j<size-1-i;j++)
		{
			if(toSort[j]>toSort[j+1])
			{
				temp = toSort[j];
				toSort[j] = toSort[j+1];
				toSort[j+1] = temp;
			}
		}
	}
}


