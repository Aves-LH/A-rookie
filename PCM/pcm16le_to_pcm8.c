#include <stdio.h>
#include <stdlib.h>

int pcm16le_to_pcm8(char *url)
{
	FILE *fp = fopen(url, "rb+");
	FILE *f1 = fopen("pcm8.pcm", "wb+");

	int cnt = 0;
	unsigned char *pcm = (unsigned char *)malloc(4);

	if(NULL != pcm)
	{
		while(!feof(fp))
		{
			short *pcmnum16 = NULL;
			char pcmnum8 = 0;
			unsigned char pcmnum8_u = 0;

			fread(pcm, 1, 4, fp);
			pcmnum16 = (short *)pcm;
			pcmnum8 = (*pcmnum16) >> 8;
			pcmnum8_u = pcmnum8 + 128;

			fwrite(&pcmnum8_u, 1, 1, f1);

			pcmnum16 = (short *)(pcm + 2);
			pcmnum8 = (*pcmnum16) >> 8;
			pcmnum8_u = pcmnum8 + 128;

			fwrite(&pcmnum8_u, 1, 1, f1);
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

	pcm16le_to_pcm8("NocturneNo2inEflat_44.1k_s16le.pcm");	
	return 0;
}
