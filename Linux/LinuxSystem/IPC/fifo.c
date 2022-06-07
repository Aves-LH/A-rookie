/*************************************************************************
	> File Name: fifo.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月15日 星期一 15时31分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	int ret = 0;
	ret = mkfifo("f1", 0644);

	if(-1 == ret)
	{
		perror("mkfifo error!");
		exit(1);
	}

	printf("mkfifo success!\n");

	return 0;
}
