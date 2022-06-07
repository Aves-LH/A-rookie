/*************************************************************************
	> File Name: exec_test.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月14日 星期日 13时54分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd = open("data.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(-1 == fd)
	{
		perror("data.txt open error!");
		exit(1);
	}


	dup2(fd, STDOUT_FILENO);
	execlp("ps", "ps", "aux", NULL);
	
	close(fd);
	return 0;
}
