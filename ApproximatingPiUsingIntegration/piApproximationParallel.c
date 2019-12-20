#include<stdio.h>
#include<omp.h>
//This code is executed parallely with 4 threads
int main(){

	double timeInit, timeFinal; 				//timeInit = time when the program starts running
				  				//timeFinal = time when program stops	
	omp_set_num_threads(4);					//number of threads is set 4
	static long totalSteps=250000000; 				//each thread will run totalSteps iterations
	double sums[4]; 					//To store value of sum for each thread
	double finalSum; 					//variable which stores the final sum(i.e. the value of pi)
	double step = 1.0/ (double)(4.0*totalSteps);		
	timeInit = 0.0;
	timeFinal =0.0;
	
	timeInit = omp_get_wtime();				// get the starting time
	#pragma omp parallel					// Parallel portion of code starts from here
	{
		int id = omp_get_thread_num();			// get the current thread ID
		
		long i = totalSteps * id + 1;			
		long end;
		double x;
		end = i + 250000000;
		sums[id]=0;
		while(i<=end){
			x = (i+0.5)*step;
			sums[id] = sums[id] + 4.0 / (1.0 + x*x);	
			i++;	
		}

	}
	finalSum = (sums[0] + sums[1] + sums[2] + sums[3]) * step;		// adding the sum obtained from 4 different threads and 
	timeFinal = omp_get_wtime();						//final time

	printf("%lf \n", finalSum);
	printf("Time taken %.10lf\n", (double)(timeFinal-timeInit));
	
}

