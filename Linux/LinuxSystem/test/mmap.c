/*************************************************************************
	> File Name: mmap.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月05日 星期日 21时43分29秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	int ret = 0;
	int fd = open("map", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}
	ftruncate(fd, 100);

	char *p = (char *)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(p == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}

	pid = fork();
	if(0 == pid)
	{
		strcpy(p, "I am child process\n");
	}
	else
	{
		sleep(1);
		printf("%s\n", p);
	}

	munmap(p, 0);
	close(fd);

	return 0;
}
