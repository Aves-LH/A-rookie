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

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void* tfn(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex2);
		printf("hello ");
		pthread_mutex_lock(&mutex2);
		sleep(2);
		printf("world\n");
		pthread_mutex_unlock(&mutex2);
		sleep(2);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	int ret = 0;

	ret = pthread_mutex_init(&mutex1, NULL);
	ret = pthread_mutex_init(&mutex2, NULL);
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
		pthread_mutex_lock(&mutex1);
		printf("HELLO ");
		pthread_mutex_lock(&mutex2);
		sleep(1);
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex1);
		sleep(1);
	}
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);


	pthread_exit(0);
}
