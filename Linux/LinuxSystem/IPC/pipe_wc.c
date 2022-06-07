/*************************************************************************
	> File Name: pipe_wc.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月15日 星期一 10时09分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int fd[2];
	pid_t pid;
	char szBuf[50] = "";
	int ret = 0;

	ret = pipe(fd);
	if(-1 == ret)
	{
		perror("pipe error!");
		exit(1);
	}
	pid = fork();
	if(0 == pid)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execlp("ls", "ls", "-l", NULL);
		close(fd[1]);
	}
	else if(pid > 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		sleep(2);
		execlp("wc", "wc", "-l", NULL);
		close(fd[0]);
	}
	else if(-1 == pid)
	{
		perror("fork error");
		exit(1);
	}

	return 0;
}
