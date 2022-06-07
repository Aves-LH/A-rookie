/*************************************************************************
	> File Name: session.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月18日 星期四 15时21分50秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	pid_t pid;

	pid = fork();
	if(-1 == pid)
	{
		perror("fork error!");
		exit(1);
	}
	else if(0 == pid)
	{
		printf("Child PID : %d\n", getpid());
		printf("Group ID : %d\n",  getpgid(0));
		printf("Session ID : %d\n", getsid(0));

		sleep(10);
		setsid();

		printf("After Changing!\n");

		printf("Child PID : %d\n", getpid());
		printf("Group ID : %d\n",  getpgid(0));
		printf("Session ID : %d\n", getsid(0));
	}

	return 0;
}
