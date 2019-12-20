/*MPI version to calculate value of PI using Mandel-Brot Method
*/

#include<stdio.h>
#include<mpi.h>
#include<math.h>
double generateRandom();
void main(int args,char *argv[])
{
	MPI_Init(&args,&argv);
	int my_rank,p,source,dest=0,tag=0;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	double n=0,total_n;
	double x,y,pi;
	long j,N;
	N=atol(argv[1]);
	double local_N=N/4;
	double initTime;
	if(my_rank==0)
		initTime=MPI_Wtime();
	for(j=0;j<local_N;j++)
	{
		x = generateRandom();
		y = generateRandom();
		if(pow(x,2)+pow(y,2)<=1)
			n++;
	}
	if(my_rank==0)
	{
		total_n=n;;
		for (source = 1; source < p; source++) 
		{
			MPI_Recv(&n, 1, MPI_FLOAT, source, tag,MPI_COMM_WORLD, &status);
			total_n = total_n + n;
		}
		pi=4*total_n/N;
	}	
	else
		MPI_Send(&n, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
	if (my_rank == 0) 
	{						
		double finalTime=MPI_Wtime();		
		printf("pi = %lf\n",pi);
		printf("time : %lf\n",finalTime-initTime);
	}
	MPI_Finalize();
}

double generateRandom()
{
	return 2 * (double)rand() / (double)2147483647 -1 ;
}
