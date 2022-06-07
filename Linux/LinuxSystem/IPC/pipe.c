/*************************************************************************
	> File Name: pipe.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月15日 星期一 09时32分11秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int ret = 0;
	int fd[2];
	char szBuf[] = "Hello father, I am child\n";
	ret = pipe(fd);
	if(-1 == ret)
	{
		perror("pipe error!");
		exit(1);
	}

	pid_t pid = fork();
	if(0 == pid)
	{
		close(fd[0]);
		sleep(2);
		write(fd[1], szBuf , strlen(szBuf));
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		int ret1 = read(fd[0], szBuf, strlen(szBuf));
		if(-1 == ret1)
		{
			perror("read error!");
			exit(1);
		}
		printf("%s", szBuf);
		close(fd[0]);
	}

	return 0;
}
