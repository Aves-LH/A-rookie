/*************************************************************************
	> File Name: fifo_r.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月05日 星期日 20时55分30秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	int ret = 0;

	//read process
	int fd = open("f1", O_RDONLY);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	while(1)
	{
		char buf[50] = "";
		read(fd, buf, sizeof(buf));
		printf("message from write : %s\n", buf);
		sleep(1);
	}

	return 0;
}
