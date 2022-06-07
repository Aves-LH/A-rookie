/*************************************************************************
	> File Name: mmap.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月16日 星期二 09时34分50秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	char *p = NULL;
	int fd;
	int ret = 0;

	//fd = open("mmaptest", O_RDWR | O_CREAT | O_TRUNC, 0644);
	fd = open("/dev/zero", O_RDWR);
	
	if(-1 == fd)
	{
		perror("open error!");
		exit(1);
	}

	p = (char *)mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(MAP_FAILED == p)
	{
		perror("mmap error!");
		exit(1);
	}

	close(fd);
	strcpy(p, "hellp mmap");

	printf("%s\n", p);

	ret = munmap(p, 20);
	
	if(-1 == ret)
	{
		perror("munmap error!");
		exit(1);
	}	

	return 0;
}
