#include<stdio.h>
#include<mpi.h>
#include<math.h>
int main(int argc,char* argv[])
{
	
	int my_rank, num_cpus;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_cpus);
	MPI_Status status;
	double initTime=MPI_Wtime();
	double sum=0.0,finalSum,pi_approx,INITTIME,FINALTIME;
	double factor=1.0;
	unsigned long n = atol(argv[1]);
	static long k;	
	for(k=my_rank*(n/4);k<(my_rank+1)*(n/4);k++)
	{
		if(k%2==0)
			factor=1.0;
		else
			factor=-1.0;
		sum+=factor/(2*k+1);
	}
	MPI_Reduce(&sum,&finalSum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	if(my_rank==0)	
	{	double pi_approx=4.0*finalSum;
	//	printf("%lf ",pi_approx);
	}
	double finalTime=MPI_Wtime();
	MPI_Reduce(&initTime,&INITTIME,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
	MPI_Reduce(&finalTime,&FINALTIME,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	if(my_rank==0)
		printf("%lf\n",FINALTIME-INITTIME);
	MPI_Finalize();
//	printf("%lf\n",pi_approx);
	
}

