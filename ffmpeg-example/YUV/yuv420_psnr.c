#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main(int argc, char **argv)
{
	yuv420_psnr("lena_256x256_yuv420p.yuv", "lena_distort_256x256_yuv420p.yuv", 256, 256, 1);	
	return 0;
}
