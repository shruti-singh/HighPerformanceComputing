//THIS IS AN OPTIMIZATION OF THE WORKING TAMPERING OF THE WORKING CODE FOR COLORFUL IMAGE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(){

	FILE *fp= fopen("lena1024.bmp", "rb");
	FILE *f = fopen("mandelplot.bmp", "wb");
	double timeInitial=omp_get_wtime();
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
	//printf("Compression : %d\n", temp);
	//exit(0);
	fseek(fp, 0,SEEK_SET);
	
	int i=0, j, var, re, flag=0;	
	int var1[]={231, 247, 3};
	//int var1[]={206, 88, 255};		
	//int var1[]={100, 150, 200};

	while(i++<(54)){

		fread(&re, sizeof(char), 1, fp);		
		fwrite(&re, 1, sizeof(char), f);

	}
	
	int color[201][3];
	int iter;
	int R=55;
	color[200][0]=255;//0;
	color[200][1]=255;//162;//255;
	color[200][2]=255;//255;//230;
	for(iter=0;iter<200;++iter)
	{

		color[iter][0]=255-iter;//5;//0;//5;
		color[iter][1]=255-iter;//iter;//255;//iter;
		color[iter][2]=255-iter;//R++;//iter;//R++;
		
		if(iter%2==0)
		{
			color[iter][0]=color[iter][2];
			int temp=color[iter][1];
			color[iter][1]=255;
			color[iter][2]=temp;
		}

	}

	int x,y,z,maxIter;

	double znX, znY, zn1, zn1X, zn1Y;
	//X and Y coordinates
	double xm, ym;

	//Initialzing Z0=0
	znX=0;

	maxIter=2000;
	
	int imageMatrix[512*2][512*2];		
	
	for (y = -256*2; y < 256*2; ++y)
	{
		for (x = -256*3; x < 256*1; ++x)
		{
				//printf("%d,%d ", y,x);
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
						imageMatrix[/*256*/512+y][/*256*/512+256+x]=(z+1+log(log(znX*znX+znY*znY))/log(z));
					}	
					else
					{	
						imageMatrix[/*256*/512+y][/*256*/512+256+x]=201;
					}

				}
				else
				{
					imageMatrix[/*256*/512+y][/*256*/512+256+x]=200;
				}

		}
		
	}
	double timeFinal=omp_get_wtime();
	printf("%lf\n",timeFinal-timeInitial);

	i=0;
	int row, col;
	
	//printf("Height: %d and Width: %d\n", height, width);
	
	for(row=0;row<height;++row)
	{	
		for(col=0;col<width;++col)
		{
			fread(&re, sizeof(char), 1, fp);
			if(imageMatrix[row][col]==201)
			{
				var=0;
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				fwrite(&re, 1, sizeof(char), f);
				fwrite(&re, 1, sizeof(char), f);
			}
			else
			{
				var=color[(imageMatrix[row][col])%200][0];
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				var=color[(imageMatrix[row][col])%200][1];
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);
				var=color[(imageMatrix[row][col])%200][2];
				re=(char)var;
				fwrite(&re, 1, sizeof(char), f);	
			}

		}
	}		
	
}

