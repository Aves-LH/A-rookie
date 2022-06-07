/*************************************************************************
	> File Name: mutex_lock.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月20日 星期六 14时10分47秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;

void* tfn(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("hello ");
		sleep(2);
		printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	int ret = 0;

	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "mutex init error : %s\n", strerror(ret));
		exit(1);
	}

	ret = pthread_create(&tid, NULL, tfn, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread create terror : %s\n", strerror(ret));
		exit(1);
	}

	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("HELLO ");
		sleep(1);
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	pthread_exit(0);
}
