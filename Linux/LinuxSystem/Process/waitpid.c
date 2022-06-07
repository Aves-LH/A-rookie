/*************************************************************************
	> File Name: fork.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月13日 星期六 17时57分35秒
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
	int n = 10;
	int i = 0;
	pid_t pid;
	printf("n = %d\n", n);


	for(i = 0; i < 5; i++)
	{
		pid_t pid1 = fork();
		if(0 == pid1)
		{
			printf("I am %d child process! PID: %d\n", i + 1, getpid());
			sleep(1);
			return 0;
		}
		if(2 == i)
		{
			pid = pid1;
			printf("child %d PID %d\n", i, pid);
		}
	}
	
	pid_t wpid = waitpid(pid ,NULL, 0);	//zuse
	//sleep(5);
	//pid_t wpid = waitpid(pid, NULL, WNOHANG);   //feizuse
	printf("child %d exit \n", pid);

	while((wpid = waitpid(-1, NULL, 0)))
	{
		printf("wait child %d\n", wpid);
	}

	return 0;
}
