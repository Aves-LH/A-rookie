#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char **argv)
{
	yuv420_gray("lena_256x256_yuv420p.yuv", 256, 256, 1);

	return 0;
}
