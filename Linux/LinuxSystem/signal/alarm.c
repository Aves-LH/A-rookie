/*************************************************************************
	> File Name: alarm.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月17日 星期三 14时55分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int i = 0;
	alarm(1);

	for(i = 0; ; i++)
	{
		printf("%d\n", i);
	}

	return 0;
}
