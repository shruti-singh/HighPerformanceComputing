#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
int main(int argc,char* argv[]){

	MPI_Init(&argc,&argv);
	double initialTime=MPI_Wtime();
	printf("%lf\n",initialTime);
	int my_rank,p,source,dest=0,tag=0;
	MPI_Status status;
	MPI_Request request;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	int imageSize=1024,x,y,z,maxIter=2000;
	double znX, znY, zn1, zn1X, zn1Y,xTemp,yTemp;
	double xm, ym;
	znX=0;
	int sizePerThread=imageSize/p;
	int imagelocal[sizePerThread][imageSize];
	for (y = (imageSize/4)*(-2)+sizePerThread*my_rank; y < (imageSize/4)*(-2)+sizePerThread*(my_rank+1); ++y)
	{
		for (x = (imageSize/4)*(-2); x < (imageSize/4)*2; ++x)
		{
				xm=(double)x/(imageSize/2);
				ym=(double)y/(imageSize/2);
				znX=znY=0;
				
				if(xm*xm + ym*ym < 1){
					
					for(z=0;z<maxIter;++z)
					{
						zn1X=znX*znX - znY*znY + xm;
						zn1Y=2*znX*znY + ym;
						xTemp=zn1X;
						yTemp=zn1Y;

						if(xTemp==znX && yTemp==znY)
						{	
							z=maxIter;
							break;
						}	
						if(((zn1X*zn1X)+(zn1Y*zn1Y)) > 4)
							break;

						if(((zn1X*zn1X)+(zn1Y*zn1Y)) > 4)
							break;

						znX=zn1X;
						znY=zn1Y;
					}
					if(z<maxIter)
						imagelocal[(imageSize/2)+y-sizePerThread*(my_rank)][(imageSize/2)+x]=z%200;
					else
						imagelocal[(imageSize/2)+y-sizePerThread*(my_rank)][(imageSize/2)+x]=201;

				}
				else
					imagelocal[(imageSize/2)+y-sizePerThread*(my_rank)][(imageSize/2)+x]=200;
				
					
		}
	}
	if(my_rank!=0)
		MPI_Isend(imagelocal, imageSize*sizePerThread, MPI_INT , dest, tag, MPI_COMM_WORLD,&request);
	double finalTime=MPI_Wtime();
	printf("%lf\n",finalTime);
	MPI_Barrier(MPI_COMM_WORLD);	
	if(my_rank==0)
	{
		int imageGlobal[imageSize][imageSize];
		int i,j;
		for(i=0;i<sizePerThread;i++)
		{
			for(j=0;j<imageSize;j++)
			{
				imageGlobal[i][j]=imagelocal[i][j];
			}
		}
		int process;
		for(process=1;process<p;process++)
		{
			MPI_Recv(imagelocal,imageSize*sizePerThread,MPI_INT,process,tag,MPI_COMM_WORLD,&status);
			printf("After MPI recv\n");
			for(i=0;i<sizePerThread;i++)
			{
				for(j=0;j<imageSize;j++)
					imageGlobal[i+sizePerThread*(process)][j]=imagelocal[i][j];
			}
		}
		FILE *fp= fopen("lena1024.bmp", "rb");
	 	FILE *f = fopen("lenaTampered1.bmp", "wb");
	 	int image[512][3];

		if (fp == NULL){
		printf("Errortoopen");
		exit(0);
		}

		if (f == NULL){
		printf("Errortocreate");
		exit(0);
		}
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
		i=0;
		int var;
		int re;

		int flag=0;	
		int var1[]={206, 88, 255};	
		while(i++<(54)){

			fread(&re, sizeof(char), 1, fp);		
			fwrite(&re, 1, sizeof(char), f);

		}
	
		int color[201][3];
		int iter;
		int R=55;
		color[200][0]=0;
		color[200][1]=162;
		color[200][2]=255;
		for(iter=0;iter<200;++iter)
		{

			color[iter][0]=0;
			color[iter][1]=255;
			color[iter][2]=iter;
			if(iter%5==0)
			{
				color[iter][0]=color[iter][2];
				int temp=color[iter][1];
				color[iter][1]=255;
				color[iter][2]=temp;
			}

		}
		i=0;
		int r, q;
		int counter=0;
		printf("Height: %d and Width: %d\n", height, width);
	
		for(r=0;r<height;++r)
		{	
			for(q=0;q<width;++q)
			{
				fread(&re, sizeof(char), 1, fp);
				if(imageGlobal[counter][q]==201)
				{
					var=0;
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);
					fwrite(&re, 1, sizeof(char), f);
					fwrite(&re, 1, sizeof(char), f);
				}
				else
				{
					var=color[(imageGlobal[counter][q])][0];
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);
					var=color[(imageGlobal[counter][q])][1];
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);
					var=color[(imageGlobal[counter][q])][2];
					re=(char)var;
					fwrite(&re, 1, sizeof(char), f);	
				}

			}
			counter=counter+1;
		}
	}
	MPI_Finalize();
}
