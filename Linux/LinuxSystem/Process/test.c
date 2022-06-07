/*************************************************************************
	> File Name: test.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月14日 星期日 19时35分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	int i = 0;

	for(i = 0; i < 5; i++)
	{
		pid = fork();
		if(0 == pid)
		{
			sleep(1);
			printf("I am %d child PID : %d\n", i + 1, getpid());
			break;
		}
	}

		sleep(5);
		printf("Parent end!!!\n");

	return 0;
}
