/*************************************************************************
	> File Name: temp.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月23日 星期二 20时52分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int i = 0;
	int fd = open("temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	for(; i < 10; i++)
	{
		if(-1 == write(fd, "Hello World!", 13))
		{
			perror("write error");
			exit(1);
		}
	}

	getchar();
	unlink("temp.txt");
	close(fd);

	return 0;
}
