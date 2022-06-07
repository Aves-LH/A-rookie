#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd = open("/dev/tty", O_RDONLY);
	int n = 0;
	char szBuf[1024] = "";

	if(-1 == fd)
	{
		perror("/dev/tty open error!");
	}

	n = read(STDIN_FILENO, szBuf, 1024);
	if(-1 == n)
	{
		perror("read error!");
	}

	write(STDOUT_FILENO, szBuf, n);

	close(fd);


	return 0;
}
