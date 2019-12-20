#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
int main(int argc,char* argv[]){

	MPI_Init(&argc,&argv);
	int my_rank,p,source,dest=0,tag=0;
	MPI_Status status;
	MPI_Request request;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	int imageSize=512,x,y,z,maxIter=2000;
	double znX, znY, zn1, zn1X, zn1Y,xTemp,yTemp;
	double xm, ym;
	znX=0;
	printf("Before creating the matrix %d\n",p);
	int sizePerThread=imageSize/(p-1);
	printf("Size per thread %d\n",sizePerThread);
	double initialTime=MPI_Wtime();
	printf("%lf\n",initialTime);
	int coorAndValues[3];
	if(my_rank!=0)
	{
		for (y = (imageSize/4)*(-2)+sizePerThread*(my_rank-1); y < (imageSize/4)*(-2)+sizePerThread*(my_rank); ++y)
		{
			for (x = (imageSize/4)*(-2); x < (imageSize/4)*2; ++x)
			{
				xm=(double)x/(imageSize/4);
				ym=(double)y/(imageSize/4);
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

						znX=zn1X;
						znY=zn1Y;
						
					}
					if(z<maxIter)
					{
					
						coorAndValues[0]=(imageSize/2)+y;
						coorAndValues[1]=(imageSize/2)+x;
						coorAndValues[2]=z%200;
						MPI_Send(coorAndValues, 3, MPI_INT , dest, tag, MPI_COMM_WORLD);
					
					}
					else
					{
						
						coorAndValues[0]=(imageSize/2)+y;
						coorAndValues[1]=(imageSize/2)+x;
						coorAndValues[2]=201;
						MPI_Send(coorAndValues, 3, MPI_INT , dest, tag, MPI_COMM_WORLD);
					
					}

				}
				else
				{
					coorAndValues[0]=(imageSize/2)+y;
					coorAndValues[1]=(imageSize/2)+x;
					coorAndValues[2]=200;
					MPI_Send(coorAndValues,3, MPI_INT , dest, tag, MPI_COMM_WORLD);
				}
				
					
			}
		}
	}
	printf("Hereeeeee\n");	

	printf("%lf\n",MPI_Wtime());
	if(my_rank==0)
	{
		FILE *fp= fopen("lena512.bmp", "rb");
	 	FILE *f = fopen("lenaTampered1.bmp", "wb");

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
		int i=0;
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
		printf("in process 0\n");
		long totalPixels=1048576,count=0;
		while(1){

			MPI_Recv(coorAndValues,3,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			int y1,x1;
			y1=coorAndValues[0];
			x1=coorAndValues[1];
			fseek(f,((y1-1)*width+x1)*sizeof(int),SEEK_SET+54);
			if(coorAndValues[2]==201)
			{
				var=0;
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				fwrite(&re, 1, sizeof(char), f);
				fwrite(&re, 1, sizeof(char), f);
				count++;
			}
			else
			{
				var=color[(coorAndValues[2])][0];
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				var=color[(coorAndValues[2])][1];
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				var=color[(coorAndValues[2])][2];
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				count++;	
			}

			if(count==totalPixels)
				break;
		}
		printf("After fillings\n");

	}
	MPI_Finalize();
}
