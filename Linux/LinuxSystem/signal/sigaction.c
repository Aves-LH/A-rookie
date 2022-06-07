/*************************************************************************
	> File Name: sigaction.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月18日 星期四 10时17分02秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

void sig_catch(int signo)
{
	printf("signal catch %d\n", signo);
	sleep(10);
	return;
}

int main(int argc, char* argv[])
{
	struct sigaction act, oldact;
	int ret = 0;

	act.sa_handler = sig_catch;
	sigemptyset(&(act.sa_mask));
	act.sa_flags = 0;

	ret = sigaction(SIGINT, &act, &oldact);
	if(-1 == ret)
	{
		perror("sigaction error!");
		exit(1);
	}

	while(1);
	return 0;
}
