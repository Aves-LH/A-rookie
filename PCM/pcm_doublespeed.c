#include <stdio.h>
#include <stdlib.h>


int pcm_doublespeed(char *url)
{
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("pcm_dblspeed.pcm", "wb+");

	unsigned char *pcm = (unsigned char *)malloc(4);
	int cnt = 0;

	if(NULL != pcm)
	{
		while(!feof(fp))
		{
			fread(pcm, 1, 4, fp);

			if(cnt % 2)
			{
				fwrite(pcm, 1, 2, f1);
				fwrite(pcm + 2, 1, 2, f1);
			}
			cnt++;
		}
	}

	free(pcm);
	fclose(fp);
	fclose(f1);

	return 0;
}

int main(int argc, char **argv)
{
	pcm_doublespeed("NocturneNo2inEflat_44.1k_s16le.pcm");

	return 0;
}
