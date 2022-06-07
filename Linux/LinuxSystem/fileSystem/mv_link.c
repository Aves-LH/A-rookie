/*************************************************************************
	> File Name: mv_link.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月12日 星期五 15时34分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int ret = link(argv[1], argv[2]);
	if(-1 == ret)
	{
		perror("link error!");
		exit(1);
	}

	ret = unlink(argv[1]);
	if(-1 == ret)
	{
		perror("link error!");
		exit(1);
	}

	return 0;
}
