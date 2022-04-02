#include <stdio.h>
#include <stdlib.h>

int pcm16le_split(char *url)
{
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("L.pcm", "wb+");
	FILE *f2 = fopen("R.pcm", "wb+");

	unsigned char *pcm = (unsigned char *)malloc(4);

	if(NULL != pcm)
	{
		while(!feof(fp))
		{
			fread(pcm, 1, 4, fp);

			fwrite(pcm, 1, 2, f1);
			fwrite(pcm + 2, 1, 2, f2);
		}
	}

	free(pcm);
	fclose(fp);
	fclose(f1);
	fclose(f2);


	return 0;
}

int main(int argc, char **argv)
{
	pcm16le_split("NocturneNo2inEflat_44.1k_s16le.pcm");

	return 0;
}
