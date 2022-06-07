/*************************************************************************
	> File Name: process.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月05日 星期日 20时07分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	pid_t pid;

	pid = fork();
	if(0 == pid)
	{
		printf("I am child process, %d\n", getpid());
		//execl("/bin/ls", "ls", "-l", NULL);
	}
	else
	{
		sleep(1);
		printf("I am parent process, %d\n", getpid());
		int status = 0;
		pid_t wpid;
		wpid = waitpid(pid, &status, WNOHANG);
		if(WIFEXITED(status))
		{
			printf("%d process waited, exit status : %d\n", wpid, WEXITSTATUS(status));
		}
	}

	return 0;
}
