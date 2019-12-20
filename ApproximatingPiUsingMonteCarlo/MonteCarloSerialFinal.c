#include<stdio.h>
#include<time.h>
#include<omp.h>
#include<math.h>
double generateRandom();
int main()
{

	double timeInit, timeFinal;

	timeInit= omp_get_wtime();

	FILE *fp= fopen("out.txt", "w");
	FILE *f= fopen("outY.txt", "w");
	
	srand(time(NULL));
	double x,y;
	long j;
	long N = 100000000;//00000;//total number of points
	double n=0;
	
	for( j=0;j<N;j++)
	{
		x = generateRandom();
		y = generateRandom();
		//if(j<1000){
		//fwrite(&x, 4, sizeof(double), fp);
		//fwrite(&y, 4, sizeof(double), f);
		//fprintf(f, "%lf ", y);
		//fprintf(fp, "%lf ", x);
		//}
		if(pow(x,2)+pow(y,2)<=1)
			n++;

	}
	double pi=4*n/N;
	
	printf("PI: %lf", pi);
	
	timeFinal= omp_get_wtime();

	printf("Time taken for serial execution: %lf", (timeFinal-timeInit));
}

double generateRandom()
{
	return 2 * (double)rand() / (double)2147483647 -1 ;
}
