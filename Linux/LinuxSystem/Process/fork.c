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

int main(int argc, char* argv[])
{
	int n = 10;
	int i = 0;
	printf("n = %d\n", n);

	pid_t pid1 = fork();
	if(0 == pid1)
	{
		printf("I am child process! PID is %d, parent PID is %d\n", getpid(), getppid());
		execl("/bin/ls", "ls", "-l", "-h", NULL);
	}
	else
	{
		sleep(2);
		printf("I am parent process! child PID is %d PID is %d, parent PID is %d\n", pid1, getpid(), getppid());
	}

	sleep(2);
	return 0;
}
