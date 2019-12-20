#include<stdio.h>
#include<mpi.h>
float Trap(float local_a,float local_b,int local_n,float h);
float f(float x);
int main(int argc, char** argv)
{
	MPI_Init(&argc,&argv);
	int my_rank,p,source,n=1024,dest=0,tag=0;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	float h,total,a=0,b=1;
	int local_n;
	h=(b-a)/n;
	local_n=n/p;
	float local_a,local_b,integral;
	local_a=a+my_rank*local_n*h;
	local_b=local_a+local_n*h;
	integral=Trap(local_a,local_b,local_n,h);
	if(my_rank==0)
	{
		total=integral;
		for (source = 1; source < p; source++) 
		{
			MPI_Recv(&integral, 1, MPI_FLOAT, source, tag,MPI_COMM_WORLD, &status);
			total = total + integral;
		}
	}
	else
		MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
	if (my_rank == 0) 
	{
		printf("With n = %d trapezoids, our estimate\n",n);
		printf("of the integral from %f to %f = %f\n",a, b, total);
	}
	MPI_Finalize();
}
float Trap(float a,float b,int n,float h)
{
	int i;
	float x_i;
	float sum=f(a);
	for(i=1;i<n;i++)
	{
		x_i=a+i*h;
		sum=sum+f(x_i);
	}
	sum=sum*h;
	return sum;
}
float f(float x)
{
	return 4.0/(1.0+x*x);
}
