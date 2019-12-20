#include<stdio.h>
#include<omp.h>
unsigned long size;
int main(int args,char* argv[])
{
	double timei = omp_get_wtime();
	size=atol(argv[1]);
	int toSort[size];
	int i;
	for(i=size; i>0;i--)
	{
		toSort[size-i] = i;
	}
	oddEvenSortParallel(toSort,size);
	printf("odd-even parallel sorted\n");
	for(i=0;i<size;i++)
	{
		printf("%d ", toSort[i]);
	}
	double timef = omp_get_wtime();
	printf("Time taken %lf", timef-timei);
	
}

void oddEvenSortParallel(int *toSort,unsigned long size)
{
	int sorted = 0;
	int i,temp;
	while(!sorted)
	{
		sorted = 1;
		#pragma omp parallel for num_threads(4) default (none) shared (sorted,toSort,size) private (i,temp)
				
		for(i=0;i<size-1;i+=2)
		{
			if(toSort[i]>toSort[i+1])
			{
				temp = toSort[i];
				toSort[i] = toSort[i+1];
				toSort[i+1] = temp;
				sorted = 0;
			}
		}	
		#pragma omp parallel for num_threads(4) default (none) shared (sorted, toSort) private (i,temp)
		for(i=1;i<size-1;i+=2)
		{
			if(toSort[i]>toSort[i+1])
			{
				temp = toSort[i];
				toSort[i] = toSort[i+1];
				toSort[i+1] = temp;				
				sorted = 0;
			}
		}
	}
}
