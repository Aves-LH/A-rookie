#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{	
	FILE *fp = NULL;
	FILE *fp_out = NULL;

	int n = 0;
	char ch = '\0'; 
	fp = fopen("news.txt", "r");
	if(NULL == fp)
	{
		perror("1 open error!");
		exit(1);
	}

	fp_out = fopen("news_cp.txt", "w+");
	if(NULL == fp_out)
	{
		perror("2 open error!");
		exit(1);
	}

	while(EOF != (n = fgetc(fp)))
	{
		fputc(n, fp_out);
	}
	
	fclose(fp);
	fclose(fp_out);

	return 0;
}
