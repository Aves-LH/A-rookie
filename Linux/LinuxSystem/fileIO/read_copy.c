#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int fd, fd_out;
	int n = 0;
	char ch[1] = "";;

	fd = open("./news.txt", O_RDONLY);
	fd_out = open("./news_cp.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);

	if(-1 == fd)
	{
		perror("1 open error!");
		exit(1);
	}

	if(-1 == fd_out)
	{
		perror("2 open error!");
		exit(1);
	}	

	while(0 != (n = read(fd, ch, 1)))
	{
		if(0 == write(fd_out, ch, n))
		{
			break;
		}
	}

	close(fd);
	close(fd_out);

	return 0;
}
