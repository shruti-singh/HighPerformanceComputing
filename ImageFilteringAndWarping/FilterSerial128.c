#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
char sort(char* temp);

int main(){

	FILE *fp= fopen("lena128.bmp", "rb");
	FILE *f = fopen("lenaFilteredNew128.bmp", "wb");

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

	int i=0, j, var;
	int re;
	int pixInit[temp];
	int pixFinal[temp];
	//int *p=pixFinal;	
	j=0;
	printf("Size: %d",temp);	
	while(++i<(54+temp)){

		fread(&re, sizeof(char), 1, fp);		
		if(i>=54)		
		{
			var=(int)re;
			//var=var/2;
			re=(char)var;
			pixInit[j++]=re;
			fwrite(&re, 1, sizeof(char), f);
		}		
		else
			fwrite(&re, 1, sizeof(char), f);

	}
	printf("Cal time\n");
	unsigned long init=100*omp_get_wtime();
	printf("%lu \n", init);
	i=0;
	j=0;
	var=0;
	char arr[128][128];
	for(i=0;i<128;++i)
		for(j=0;j<128;++j)
			arr[i][j]=pixInit[var++];
	char temp1[49];				
	int m,n,o,p;
	int IS=0;
	int w,z;
	for(m=0;m<128;++m)
	{
		for(n=0;n<128;++n)
		{
			if(m>=3 && n>=3 && m<=124 && n<=124)
			{
				IS=0;
				for(w=m-3;w<=(m+3);++w)
					for(z=n-3;z<=(n+3);++z)
					{	//printf("Here %d %d\n",w, z);	
						temp1[IS++]=arr[w][z];	
					}
				arr[m][n]=sort(temp1);

			}
		}				
	}

	i=0;
	fseek(f, 54, SEEK_SET);
	for(m=0;m<128;++m)
	{
		for(n=0;n<128;++n)
		{
			re=arr[m][n];
			fwrite(&re, 1, sizeof(char), f);
		}
	}
	unsigned long final=100*omp_get_wtime();
	printf("diff %lu ",(final-init));

}

char sort(char* temp){

	int j,k, min;

	for(j=0;j<49;++j)
	{
		min=temp[j];
		for(k=j;k<49;++k)
		{
			if(temp[k]<min)
			{
				min=temp[k];
			}
		}
		temp[j]=min;	
	}

	return temp[24];
}
