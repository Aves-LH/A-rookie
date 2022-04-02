#include <stdio.h>
#include <stdlib.h>

int pcm_halfleftvolume(char *url)
{	
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("L_halfvolume.pcm", "wb+");

	int cnt = 0;
	unsigned char *pcm = (unsigned char *)malloc(4);

	if(NULL != pcm)
	{
		while(!feof(fp))
		{
			short *pcmnum = NULL;
			fread(pcm, 1, 4, fp);

			pcmnum = (short *)pcm;
			*pcmnum = *pcmnum / 2;

			fwrite(pcm, 1, 2, f1);
			fwrite(pcm + 2, 1, 2, f1);
			cnt++;
		}
	}
	printf("pcm cnt: %d \n", cnt);

	free(pcm);
	fclose(fp);
	fclose(f1);

	return 0;
}

int main(int argc, char **argv)
{
	pcm_halfleftvolume("NocturneNo2inEflat_44.1k_s16le.pcm");

	return 0;
}
