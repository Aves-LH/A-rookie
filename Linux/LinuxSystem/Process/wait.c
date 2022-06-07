/*************************************************************************
	> File Name: wait.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月14日 星期日 16时51分19秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	int status = 0;

	pid =fork();
	if(-1 == pid)
	{
		perror("fork error!");
		exit(1);
	}
	else if(pid == 0)
	{
		printf("I am child ID : %d\n", getpid());
		sleep(5);
		printf("Child terminated!\n");
		return 100;
	}
	else
	{
		printf("I am parent ID : %d child PID : %d\n", getpid(), pid);
		pid_t wpid = wait(&status);
		if(WIFEXITED(status))
		{
			printf("child exit with %d\n", WEXITSTATUS(status));
		}

		if(WIFSIGNALED(status))
		{
			printf("child exit with %d\n", WTERMSIG(status));
		}
	}
	return 0;
}
