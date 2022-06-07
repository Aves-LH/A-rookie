/*************************************************************************
	> File Name: chmod.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月12日 星期五 15时27分29秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	int ret = chmod("news.txt", S_IRUSR | S_IWUSR | S_IXUSR);
	if(-1 == ret)
	{
		perror("chmod error!");
		exit(1);
	}

	printf("chmod success!\n");	

	return 0;
}
