/*************************************************************************
	> File Name: stat.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月23日 星期二 20时26分56秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	struct stat sb;
	int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	if(-1 == lstat(argv[1], &sb))
	{
		perror("stat error");
		exit(1);
	}

	if(S_ISSOCK(sb.st_mode))
	{
		printf("It is a socket file!\n");
	
	}
	if(S_ISLNK(sb.st_mode))
	{
		printf("It is a symbolic link!\n");
	}
	if(S_ISREG(sb.st_mode))
	{
		printf("It is a regular file!\n");
	}
	if(S_ISBLK(sb.st_mode))
	{
		printf("It is a block device!\n");
	}
	if(S_ISDIR(sb.st_mode))
	{
		printf("It is a directory!\n");
	}
	if(S_ISCHR(sb.st_mode))
	{
		printf("It is a character device!\n");
	}
	if(S_ISFIFO(sb.st_mode))
	{
		printf("It is a FIFO!\n");
	}


	return 0;
}
