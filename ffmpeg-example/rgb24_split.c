#include <stdio.h>
#include <stdlib.h>

int rgb24_split(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	if(NULL == fp)
	{
		perror("fopen error");
	}
	FILE *f1 = fopen("R.rgb", "wb+");
	FILE *f2 = fopen("G.rgb", "wb+");
	FILE *f3 = fopen("B.rgb", "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 3, fp);

			for(int j = 0; j < w * h * 3; j += 3)
			{
				fwrite(pic + j, 1, 1, f1);
				fwrite(pic + j + 1, 1, 1, f2);
				fwrite(pic + j + 2, 1, 1, f3);
			}
		}
	}
	free(pic);
	fclose(fp);
	fclose(f1);
	fclose(f2);
	fclose(f3);

	return 0;
}

int yuv420_split(char *url, int w, int h , int num)
{
	FILE *fp = fopen(url, "wb+");
	FILE *f1 = fopen("Y_420.yuv", "wb+");
	FILE *f2 = fopen("U_420.yuv", "wb+");
	FILE *f3 = fopen("V_420.yuv", "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 3 / 2, fp);

			fwrite(pic, 1, w * h, f1);
			fwrite(pic + w * h, 1, w * h / 4, f2);
			fwrite(pic + w * h * 5 / 4, 1, w * h / 4, f3);
		}
	}

	free(pic);
	fclose(fp);
	fclose(f1);
	fclose(f2);
	fclose(f3);

	return 0;
}

int yuv420_border(char *url, int w, int h, unsigned char border, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("yuv420_border.yuv", "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 3 / 2, fp);

			for(int m = 0; m < w; m++)
			{
				for(int n = 0; n < h; n++)
				{
					if((m < border || (m > (w - border))) || (n < border || (n < (h - border))))
					{
						pic[m * w + n] = 255;
					}
				}
			}

			fwrite(pic, 1, w * h * 3 / 2, f1);
		}
	}

	free(pic);
	fclose(fp);
	fclose(f1);

	return 0;
}

int yuv420_gray(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("yuv420_gray.yuv", "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 3 / 2, fp);

			memset(pic + w * h, 128, w * h / 2);
			fwrite(pic, 1, w * h * 3 / 2, f1);
		}
	}

	free(pic);
	fclose(fp);
	fclose(f1);

	return 0;
}

int simplest_yuv420_graybar(int width, int height,int ymin,int ymax,int barnum,char *url_out){
	int barwidth;
	float lum_inc;
	unsigned char lum_temp;
	int uv_width,uv_height;
	FILE *fp=NULL;
	unsigned char *data_y=NULL;
	unsigned char *data_u=NULL;
	unsigned char *data_v=NULL;
	int t=0,i=0,j=0;
 
	barwidth=width/barnum;
	lum_inc=((float)(ymax-ymin))/((float)(barnum-1));
	uv_width=width/2;
	uv_height=height/2;

	data_y=(unsigned char *)malloc(width*height);
	data_u=(unsigned char *)malloc(uv_width*uv_height);
	data_v=(unsigned char *)malloc(uv_width*uv_height);

	if((fp=fopen(url_out,"wb+"))==NULL){
		printf("Error: Cannot create file!");
		return -1;
	}

	//Output Info
	printf("Y, U, V value from picture's left to right:\n");
	for(t=0;t<(width/barwidth);t++){
		lum_temp=ymin+(char)(t*lum_inc);
		printf("%3d, 128, 128\n",lum_temp);
	}

	for(j=0;j<height;j++){
		for(i=0;i<width;i++){
			t=i/barwidth;
			lum_temp=ymin+(char)(t*lum_inc);
			data_y[j*width+i]=lum_temp;
		}
	}

	for(j=0;j<uv_height;j++){
		for(i=0;i<uv_width;i++){
			data_u[j*uv_width+i]=128;
		}
	}

	for(j=0;j<uv_height;j++){
		for(i=0;i<uv_width;i++){
			data_v[j*uv_width+i]=128;
		}
	}

	fwrite(data_y,width*height,1,fp);
	fwrite(data_u,uv_width*uv_height,1,fp);
	fwrite(data_v,uv_width*uv_height,1,fp);
	fclose(fp);
	free(data_y);
	free(data_u);
	free(data_v);
	return 0;
}

int yuv420_halfy(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("yuv420_halfy.yuv", "wb+");

	unsigned char *pic=  (unsigned char *)malloc(w * h *3 / 2);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 3 / 2, fp);

			for(int j = 0; j < w * h; j++)
			{
				unsigned char tmp = pic[j] / 2;
				pic[j] = tmp;
			}

			fwrite(pic, 1, w * h * 3 / 2, f1);
		}
	}

	free(pic);
	fclose(fp);
	fclose(f1);

	return 0;
}

int yuv420_psnr(char *url1, char *url2, int w, int h, int num)
{
	FILE *f1 = fopen(url1, "rb+");
	FILE *f2 = fopen(url2, "rb+");
	unsigned char *pic1 = (unsigned char *)malloc(w * h);
	unsigned char *pic2 = (unsigned char *)malloc(w * h);

	for(int i = 0; i < num; i++)
	{
		fread(pic1, 1, w * h, f1);
		fread(pic2, 1, w * h, f2);

		double mse_sum = 0;
		double mse = 0;
		double psnr = 0;

		for(int j = 0; j < w * h; j++)
		{
			mse_sum += pow((double)(pic1[j] - pic2[j]), 2);
		}

		mse = mse_sum / (w * h);
		psnr = 10 * log10(255.0 * 255.0 / mse);
		printf("%5.3f\n", psnr);

		fseek(f1, w * h / 2, SEEK_CUR);
		fseek(f2, w * h / 2, SEEK_CUR);
	}
	
	free(pic1);
	free(pic2);
	fclose(f1);
	fclose(f2);

	return 0;
}

int yuv422_split(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "wb+");
	FILE *f1 = fopen("Y_422.yuv", "wb+");
	FILE *f2 = fopen("U_422.yuv", "wb+");
	FILE *f3 = fopen("V_422.yuv", "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 2);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 2, fp);

			fwrite(pic, 1, w * h, f1);
			fwrite(pic + w * h, 1, w * h / 2, f2);
			fwrite(pic + w * h * 3 / 2, 1, w * h / 2, f3);
		}
	}

	free(pic);
	fclose(fp);
	fclose(f1);
	fclose(f2);
	fclose(f3);

	return 0;
}

int yuv444_split(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "wb+");
	FILE *f1 = fopen("Y444.yuv", "wb+");
	FILE *f2 = fopen("U444.yuv", "wb+");
	FILE *f3 = fopen("V444.yuv", "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3);

	if(NULL != pic)
	{
		for(int i = 0; i < num; i++)
		{
			fread(pic, 1, w * h * 3, fp);

			fwrite(pic, 1, w * h, f1);
			fwrite(pic + w * h, 1, w * h, f2);
			fwrite(pic + w * h * 2, 1, w * h, f3);
		}
	}

	free(pic);
	fclose(fp);
	fclose(f1);
	fclose(f2);
	fclose(f3);

	return 0;
}



int main(int argc, char **argv)
{
	rgb24_split("cie1931_500x500.rgb", 500, 500, 1);

	return 0;
}
