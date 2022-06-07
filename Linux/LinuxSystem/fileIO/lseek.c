#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	int fd, n;
	char msg[] = "This is a test for lseek\n";
	char ch = '\0';

	fd = open("lseek.txt", O_CREAT | O_RDWR, 0644);
	if(-1 == fd)
	{
		perror("open lseek.txt error!");
	}

	write(fd, msg, strlen(msg));

	lseek(fd, 0, SEEK_SET);

	while((n = read(fd, &ch, 1)))
	{
		if(-1 == n)
		{
			perror("read error!");
			exit(1);
		}
		write(STDOUT_FILENO, &ch, n);
	}

	close(fd);

	return 0;
}
