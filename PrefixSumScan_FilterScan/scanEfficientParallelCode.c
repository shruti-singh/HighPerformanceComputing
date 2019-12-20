//scan efficient parallel

#include<stdio.h>
#include<omp.h>
#include<math.h>
int main(int argc, char *argv[])
{
	long size= atol(argv[1]);
	long i,t,k;
	omp_set_num_threads(4);
	long x[size];
	for(i=0;i<size;i++)
	{
		x[i]=i+1;
	}
	double end = log10((double)size)/log10(2);
	double initTime,finalTime;
	initTime = omp_get_wtime();
	long d=1, d1;
	for(i=0;i<end;i++)
	{
		d=d<<1;
		d1=d>>1;
		#pragma omp parallel for schedule(dynamic,1) private(k)
		for(k=0;k<size;k=k+d)
		{
			x[k+d-1]=x[k+d1-1]+x[k+d-1];
		}
	}	
		
	x[size-1]=0;
	d=1;
	for(i=end-1;i>=0;i--)
	{
		d=d<<1;
		d1=d>>1;		
		#pragma omp parallel for  private(k)
		for(k=0;k<size;k=k+d)
		{
			
			t=x[k+d1-1];
			x[k+d1-1]=x[k+d-1];
			x[k+d-1]=t+x[k+d-1];
			
					
		}
	}

	finalTime=omp_get_wtime();
	printf("%lf %lf\n",log10((double)size),finalTime-initTime);
}
