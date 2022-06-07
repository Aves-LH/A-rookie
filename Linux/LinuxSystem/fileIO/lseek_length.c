#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd = open(argv[1], O_RDWR);
	int length = 0;

	if(-1 == fd)
	{
		perror("open error!");
	}

	length = lseek(fd, 0, SEEK_END);
	printf("file size : %d\n", length);

	close(fd);


	return 0;
}
