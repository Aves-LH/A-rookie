#include <stdio.h>
#include <stdlib.h>


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


int main(int argc, char **argv)
{
	yuv420_border("lena_256x256_yuv420p.yuv", 256, 256, 20, 1);

	return 0;
}
