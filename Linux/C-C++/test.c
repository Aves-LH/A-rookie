/*************************************************************************
	> File Name: test.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月18日 星期四 14时52分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd = open("a", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	printf("open success!\n");

	return 0;
}
