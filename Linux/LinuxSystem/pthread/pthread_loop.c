/*************************************************************************
	> File Name: pthread_loop.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月19日 星期五 08时27分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* tfn(void *arg)
{
	int i = (int)arg;
	sleep(i);

	printf("----I %d am thread : pid = %d, tid = %lu\n",i + 1,  getpid(), pthread_self());
	return NULL;
}

int main(int argc, char* argv[])
{
	int ret = 0;
	pthread_t tid;
	int i = 0;

	for(; i < 5; i++)
	{
		ret = pthread_create(&tid, NULL, tfn, (void *)i);
		if(0 != ret)
		{
			perror("pthread_create error!");
			exit(1);
		}
	}


	printf("main: I am Main, pid = %d, tid = %lu\n", getpid(), pthread_self());
	pthread_exit((void *)0);
	return 0;
}
