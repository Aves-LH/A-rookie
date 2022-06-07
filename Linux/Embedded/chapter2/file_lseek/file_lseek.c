#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int temp, seektemp, i, j;
	int filefd;

	char wbuf[16] = "This is a test!\n";

	if(2 != argc)
	{
		printf("run error!\n");
		return 1;
	}

	filefd = open(*(argv + 1), O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
	temp = write(filefd, wbuf, sizeof(wbuf));
	seektemp = lseek(filefd, 0, SEEK_CUR);

	for(i = 0; i < 10; i++)
	{
		j = sizeof(wbuf) * (i + 1);

		seektemp = lseek(filefd, j, SEEK_SET);
		temp = write(filefd, wbuf, sizeof(wbuf));
	}
	close(filefd);
	return 0;
}
