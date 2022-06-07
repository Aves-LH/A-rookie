/*************************************************************************
	> File Name: access.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月12日 星期五 15时23分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int ret = access("news.txt", W_OK);
	if(-1 == ret)
	{
		perror("access error!");
		exit(1);
	}

	printf("The file can write!\n");
	return 0;
}
