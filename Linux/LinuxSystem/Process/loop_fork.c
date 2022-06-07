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
		pid_t pid = fork();
		if(0 == pid)
		{
			printf("I am %d child process! PID: %d\n", i + 1, getpid());
			sleep(1);
			return 0;
		}
	}
	

	return 0;
}
