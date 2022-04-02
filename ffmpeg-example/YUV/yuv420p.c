#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char **argv)
{
	yuv420_split("lena_256x256_yuv420p.yuv", 256, 256, 1);

	return 0;
}
