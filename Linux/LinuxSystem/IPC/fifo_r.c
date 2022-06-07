/*************************************************************************
	> File Name: fifo_w.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月15日 星期一 15时50分14秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd;
	char szBuf[1024] = "";
	int i = 0;

	if(argc != 2)
	{
		printf("Usage: %s fifoname\n", argv[0]);
	}
	
	fd = open(argv[1], O_RDONLY);
	if(-1 == fd)
	{
		perror("open error!");
		exit(1);
	}

	while(1)
	{
		int len = read(fd, szBuf, 1024);
		//write(STDOUT_FILENO, szBuf, len);
		printf("%s", szBuf);
		sleep(3);
	}

	return 0;
}
