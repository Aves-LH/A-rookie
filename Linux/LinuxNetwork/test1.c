/*************************************************************************
	> File Name: test1.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月27日 星期六 20时30分20秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int i = 0;
	for(i = 0; i < 4; i++);

	printf("i = %d\n", i);

	return 0;
}
