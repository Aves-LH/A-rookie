/*************************************************************************
	> File Name: daemon.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月20日 星期六 09时52分47秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	int ret = 0;
	int fd;

	pid = fork();
	if(pid > 0)
	{
		exit(0);
	}

	pid = setsid();

	ret = chdir("/home/aves");
	if(-1 == ret)
	{
		perror("chdir error!");
		exit(1);
	}

	umask(0022);

	close(0);

	fd = open("/dev/null", O_RDWR);
	if(-1 == fd)
	{
		perror("open error!");
		exit(1);
	}
	dup2(STDOUT_FILENO, fd);
	dup2(STDIN_FILENO, fd);

	while(1);

	return 0;
}
