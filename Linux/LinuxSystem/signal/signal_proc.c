/*************************************************************************
	> File Name: signal_proc.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月18日 星期四 08时49分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

void print_set(sigset_t *pedset)
{
	int i = 0;

	for(; i < 32; i++)
	{
		if(sigismember(pedset, i))
		{
			putchar('1');
		}
		else
		{
			putchar('0');
		}
	}
	putchar('\n');
}

int main(int argc, char* argv[])
{
	sigset_t set, oldset, pedset;
	int ret = 0;

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGBUS);
	sigaddset(&set, SIGKILL);

	ret = sigprocmask(SIG_BLOCK, &set, &oldset);
	if(-1 == ret)
	{
		perror("sigprocmask error!");
		exit(1);
	}

	while(1)
	{
		ret = sigpending(&pedset);
		if(-1 == ret)
		{
			perror("sigpending error!");
			exit(1);
		}

		print_set(&pedset);
		sleep(1);
	}

	return 0;
}
