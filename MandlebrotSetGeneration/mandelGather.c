//This is an MPI version of computing the mandelbrot set.
/*
* The image is divided into chunks of equal sizes, depending on the number of processors we have to work with.
* Each processor reads the image, processes the coordinate of the pixel, and stores the pixel value in a matrix, imageMatrixPartial.
* Using Gather, data from imageMatrixPartial is sent to the root process(rank=0), which stores it in imageMatrixFinal and assigns the RGB values 
* and constructs the final image. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc,char* argv[]){

	int my_rank, commSize, source, dest=0, tag=0;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);

	FILE *fp = fopen("lena1024.bmp", "rb");
	FILE *f = fopen("mandelbrot.bmp", "wb");
	
	int temp=0;

	fseek(fp, 18,SEEK_SET);
	fread(&temp, sizeof(int), 1, fp);
	int width = temp;

	fseek(fp, 22,SEEK_SET);
	fread(&temp, sizeof(int), 1, fp);
	int height = temp;
	
	fseek(fp, 34,SEEK_SET);
	fread(&temp, sizeof(int), 1, fp);

	fseek(fp, 0,SEEK_SET);
	
	int i=0, j, var, re, flag=0;	
	
	//Copy the header in process 0
	while(i++<(54)){

		fread(&re, sizeof(char), 1, fp);		
		if(my_rank==0)
			fwrite(&re, 1, sizeof(char), f);

	}
	
	//Define RGB values for different sections of the image
	int var1[]={255, 255, 9};	
	int color[201][3];
	int iter;
	int R=55;
	//color[200] is for the outer region
	color[200][0]=0;
	color[200][1]=255;
	color[200][2]=230;
	//0,255,230 is yellow.

	for(iter=0;iter<200;++iter)
	{

		color[iter][0]=255;//5;//0;//5;
		color[iter][1]=iter;//255;//iter;
		color[iter][2]=R++;//iter;//R++;
	
		if(iter%2==0)
		{
			color[iter][0]=color[iter][2];
			int temp=color[iter][1];
			color[iter][1]=255;
			color[iter][2]=temp;
		}

	}


	int imageSize=1024, maxIter=2000, z;
	long x,y;

	//Complex coordinates
	double znX, znY, zn1, zn1X, zn1Y;

	//Coordinates in real plane
	double xm, ym;
	
	//Initialize Z0=0
	znX=0;

	//coordinates for the matrix
	long xcord, ycord;
	xcord=ycord=0;

	int sizePerThread=imageSize/commSize;

	int *imageMatrixPartial=(int*)calloc(262144, sizeof(int));

	for (y = -256*2+(1024*256)*my_rank; y < (-256*2+(1024*256)*(my_rank+1)); ++y)
	{
			
			for (x = -256*2; x < 256*2; ++x)
			{	
	
				xm=(double)x/512.0;
				ym=(double)y/512.0;
				znX=znY=0;
				
				//printf("xm: %lf ym:%lf\n", xm, ym);
				if(xm*xm + ym*ym < 2){
					
					for(z=0;z<maxIter;++z)
					{
						zn1X=znX*znX - znY*znY + xm;
						zn1Y=2*znX*znY + ym;
						if(((zn1X*zn1X)+(zn1Y*zn1Y)) > 4)
							break;

						znX=zn1X;
						znY=zn1Y;
					}
					
					if(z<maxIter)
					{
						imageMatrixPartial[ycord+xcord]=z%200;
					}
					else
					{
						imageMatrixPartial[ycord+xcord]=201;
					}

				}
				else
				{
					imageMatrixPartial[ycord+xcord]=200;
				}
				
				//printf("RANK:%d   Y:%ld X:%ld Value:%d  ",my_rank, ycord, xcord,imageMatrixPartial[ycord+xcord] );
				//printf("X=%ld Y=%ld %d", xcord, ycord, imageMatrixPartial[ycord+xcord]);
				//printf("What my_rank= %d   Y= %ld   X= %ld\n", my_rank, ycord,xcord);
				++xcord;
			}
			
			++ycord;
	}
	
	//printf("Matrix cakcukated FOR RANK %d\n", my_rank);

	int *imageMatrixFinal=NULL;
	if (my_rank == 0) {
	  imageMatrixFinal = (int*)calloc(262144*4, sizeof(int));

	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	int MPI_err=MPI_Gather(imageMatrixPartial, 262144, MPI_INT, imageMatrixFinal, 262144, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	free(imageMatrixPartial);

	if(my_rank==0)
	{

		int row, col;

		//printf("FOR IMAGE: Height: %d and Width: %d\n", height, width);
		
		for(row=0;row<height;++row)
		{	
			for(col=0;col<width;++col)
			{
				fread(&re, sizeof(char), 1, fp);
			
				if(imageMatrixFinal[row+col]==201)
				{
					
					var=0;
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);
					fwrite(&re, 1, sizeof(char), f);
					fwrite(&re, 1, sizeof(char), f);
				}
				else
				{
					var=color[(imageMatrixFinal[row+col])][0];
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);
					var=color[(imageMatrixFinal[row+col])][1];
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);
					var=color[(imageMatrixFinal[row+col])][2];
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);	
				}
			
			}

		}
		free(imageMatrixFinal);		
	}
		
	

	//printf("my_rank is %d and I am exiting\n", my_rank);
	MPI_Finalize();
}

