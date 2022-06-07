#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int filefd, temp1;
	int i = 0;
	char wbuf[16] = "This is a test!\n";

	filefd = open(*(argv + 1), O_RDWR | O_CREAT, S_IRWXU);
	printf("filefd = %d\n", filefd);

	for(i = 0; i < 10; i++)
	{
		temp1 = write(filefd, wbuf, 16);

		printf("%d\n", temp1);
	}
	close(filefd);
	return 0;
}
