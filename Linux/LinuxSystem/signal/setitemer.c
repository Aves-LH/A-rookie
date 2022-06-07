/*************************************************************************
	> File Name: setitemer.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月17日 星期三 15时24分11秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>

int main(int argc, char* argv[])
{
	struct itimerval it, oldit;
	int i = 0;

	it.it_value.tv_sec = 1;
	it.it_value.tv_usec = 0;

	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;

	if(-1 == setitimer(ITIMER_REAL, &it, &oldit))
	{
		perror("setitimer error!");
		return -1;
	}

	for(i = 0; ; i++)
	{
		printf("%d\n", i);
	}

	return 0;
}
