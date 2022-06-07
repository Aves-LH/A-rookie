/*************************************************************************
	> File Name: process_loop.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月05日 星期日 21时35分04秒
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
	int i = 0;

	for(i = 0; i < 5; i++)
	{
		pid = fork();
		if(0 == pid)
		{
			printf("I am %d child process!\n", getpid());
			return 0;
		}
		else
		{
			sleep(2);
			pid_t wpid;
			int status = 0;
			wpid = waitpid(-1, &status, WNOHANG);
			if(WIFEXITED(status))
			{
				printf("%d chlid process wait, status: %d\n",wpid,  WEXITSTATUS(status));
			}
		}
	}

	/*sleep(2);
	pid_t wpid;
	int status = 0;
	wpid = waitpid(-1, &status, WNOHANG);
	if(WIFEXITED(status))
	{
		printf("%d chlid process wait, status: %d\n",wpid,  WEXITSTATUS(status));
	}*/

	return 0;
}
