#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
void warp(int width, int height, int *inputImage, int *outputImage);
int bilinearlyInterpolate(int *inputImage, int width, int height, int x, int y);
int main(){

	FILE *fp= fopen("lena512.bmp", "rb");
	FILE *f = fopen("lena512para.bmp", "wb");

	if (fp == NULL){
		printf("Errortoopen");
		exit(0);
	}

	if (f == NULL){
		printf("Errortocreate");
		exit(0);
	}
	int temp;

	fseek(fp, 18,SEEK_SET);
	fread(&temp, sizeof(int), 1, fp);
	int width = temp;

	fseek(fp, 22,SEEK_SET);
	fread(&temp, sizeof(int), 1, fp);
	int height = temp;
	int MATRIXSIZE=height*width;
	fseek(fp, 34,SEEK_SET);
	fread(&temp, sizeof(int), 1, fp);
	
	fseek(fp, 0,SEEK_SET);

	int i=0, j, var;
	int re;
	int pixInit[temp];
	int pixFinal[temp];
	int *p=pixFinal;	
	j=0;	
	while(++i<(54+temp)){

		fread(&re, sizeof(char), 1, fp);		
		if(i>=54)		
		{
			var=(int)re;
			re=(char)var;
			pixInit[j++]=re;
			fwrite(&re, 1, sizeof(char), f);
		}		
		else
			fwrite(&re, 1, sizeof(char), f);

	}
	warp(width,height,pixInit,pixFinal);
	char temp2;
	fseek(f,54,SEEK_SET);
	for(i=0;i<MATRIXSIZE;i++)
	{
		temp2=(char)pixFinal[i];
		fwrite(&temp2,sizeof(char),1,f);
	}
	
	fclose(fp);
	fclose(f);
	
}

void warp(int width, int height, int *inputImage, int *outputImage)
{
	int index, xp, yp;/*we use xp and yp to traverse all pixels*/
	int tx = width/2, ty = height/2;
	int x, y; 
	float radius, theta, PI = 3.141527f;
/*	DRAD = PI/180;*/
	int xi, yi;
	int rmax = width/2;
	omp_set_num_threads(2);
	double initTime = omp_get_wtime();
	#pragma omp parallel for shared(inputImage, outputImage, height, width) private(x, y, index, radius, xp,yp)
	for(yp = 0; yp<height;yp++)
	{
		for(xp = 0; xp<width;xp++)
		{
			index = xp + yp * width;
			if(xp!=tx&&yp!=ty)
			{	radius = sqrtf((xp - tx) * (xp - tx) + (yp - ty) * (yp - ty));
				theta = atan2((xp-tx),(yp-ty))+PI*((rmax-radius)/rmax)/2;
				/*converting from degree to radian
//				x = (int)(floor(cos(theta) * (xp - tx) - sin(theta) * (yp - ty) + tx));
//				y = (int)(floor(sin(theta) * (xp - tx) + cos(theta) * (yp - ty) + ty));*/
				if(radius<=rmax)
					x=(int)(floor(tx+radius*cos(theta)));
				else
					x=xp;
				if(radius<=rmax)
					y=(int)(floor(ty+radius*sin(theta)));
				else
					y=yp;
				/*if(x<0)
					x=0;
				else if(x>=width)
					x=width-1;
				if(y<0)
					y=0;
				else if(y>=height)
					y=height-1;*/
				outputImage[index] = inputImage[x+y*width];/*bilinearlyInterpolate(inputImage, width, height, x, y);*/
			}
 	
		}
	}
	printf("%d %lf",8,omp_get_wtime()-initTime);
/*	for(yp = 0; yp<height;yp++)
	{
		for(xp = 0; xp<width;xp++)
		{
			printf("%d ",outputImage[xp + yp * width]);
		}
		printf("\n");
	}*/	
	
}
int bilinearlyInterpolate(int *inputImage, int width, int height, int x, int y)
{
	int x2,y2;
	/*int x1 = x;
	if(x1 - x != 0 && x1 + 1< width)
	{
		x1 = x1 + 1;
	}
	int y1 = y;
	if(y1 - y != 0 & y1 + 1 <height)
	{
		y1 = y1 + 1;
	}*/
	int sumOfIntensities = 0;
	sumOfIntensities += inputImage[x + y * width];
	if(x + 1 < width)
	{
		x2 = x + 1;

	}
	else
	{
		x2 = x - 1;
	}
	sumOfIntensities+= inputImage[x2 + y * width];
		
	if(y + 1 <height)
	{
		y2 = y + 1;
	}
	else
	{
		y2 = y - 1;
	}
	sumOfIntensities += inputImage[x + y2 * width];
	sumOfIntensities += inputImage[x2 + y2 * width];
	sumOfIntensities /= 4;
	return sumOfIntensities;
	
}

/*void warp(int width, int height, int *inputImage, int *outputImage)
{
	float PI=3.141527f;
	int x,y,size,radius,cx,cy,destPos,sourcePos,x0,y0,xf,yf,pos0,pos1,pos2,pos3,componentX0,componentX1,finalPixelComponent,k;
	float r,alpha,degrees,newX,newY,deltaX,deltaY;
	cx=floor(width/2);
	cy=floor(height/2);
	size=width<height?width:height;
	radius=floor(size/4);
	for(y=-radius;y<radius;++y)
	{
		for(x=-radius;x<radius;++x)
		{
			if(x*x+y*y<=radius*radius)		
			{
				r=sqrt(x*x+y*y);
				alpha=atan2(y,x);
				destPos = (y + cy) * width + x + cx;
                    		destPos *= 4;	
				degrees=(alpha*180)/PI;
				alpha = (degrees*PI)/180.0;
				newX=r*sin(alpha);
				newY=r*cos(alpha);
				sourcePos=(newX+cy)*width+newX+cx;
				sourcePos*=4;

				outputImage[destPos + 0] = inputImage[sourcePos + 0];
                   		outputImage[destPos + 1] = inputImage[sourcePos + 1];
                   		outputImage[destPos + 2] = inputImage[sourcePos + 2];
                    		outputImage[destPos + 3] = inputImage[sourcePos + 3];
				
			/*	x0 = floor(newX); 
				xf = x0 + 1;
				y0 = floor(newY); 
				yf = y0 + 1;
				deltaX = newX - x0;
				deltaY = newY - y0;

				pos0 = ((y0 + cy) * width + x0 + cx) * 4;
				pos1 = ((y0 + cy) * width + xf + cx) * 4;
				pos2 = ((yf + cy) * width + x0 + cx) * 4;
				pos3 = ((yf + cy) * width + xf + cx) * 4;

				for (k = 0; k < 4; ++k) 
				{
    					componentX0 = (inputImage[pos1 + k] - inputImage[pos0 + k]) * deltaX + inputImage[pos0 + k];
   					componentX1 = (inputImage[pos3 + k] - inputImage[pos2 + k]) * deltaX + inputImage[pos2 + k];
    					finalPixelComponent = (componentX1 - componentX0) * deltaY + componentX0;
  					outputImage[destPos + k] = finalPixelComponent > 255 ? 255 : (finalPixelComponent < 0 ? 0 : finalPixelComponent);
 
				}
			}
		}		
	}
}*/



