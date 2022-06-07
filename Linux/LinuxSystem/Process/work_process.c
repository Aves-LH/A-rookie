/*************************************************************************
	> File Name: work_process.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月15日 星期一 08时26分14秒
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
	int status = 0;
	int i = 0;
	pid_t wpid;

	for(i = 0; i < 3; i++)
	{
		pid_t pid = fork();
		if(0 == pid)
		{
			if(0 == i)
			{
				execl("ps", "ps", "aux", NULL);
				return 0;
			}
			else if(1 == i)
			{
				execl("hello", "hello", "", NULL);
				return 0;
			}
			else if(2 == i)
			{
				execl("error", "error", "", NULL);
				return 0;
			}
		}
	}

	for(i = 0;i < 3 ; i++)
	{
		(wpid = waitpid(-1, NULL, 0));
		printf("wait child %d\n", wpid);
	}

	return 0;
}
