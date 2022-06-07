/*************************************************************************
	> File Name: signal.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月18日 星期四 09时26分40秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

void sighandler(int signo)
{
	printf("signal catch %d\n", signo);
}

int main(int argc, char* argv[])
{
	signal(SIGINT, sighandler);
	while(1);

	return 0;
}
