/*************************************************************************
	> File Name: sem_producer_consumer.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月21日 星期日 15时21分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t producer_num;
sem_t consumer_num;
#define NUM 5
int nArr[NUM];

void* producer(void* arg)
{
	int i = 0;

	while(1)
	{
		sem_wait(&consumer_num);
		nArr[i] = rand() % 1000 + 1;
		printf("Producer---------%d\n", nArr[i]);
		sem_post(&producer_num);
		i = (i + 1) % NUM;
		sleep(2);
	}

	return NULL;
}

void* consumer(void *arg)
{
	int i = 0;

	while(1)
	{
		sem_wait(&producer_num);
		printf("Consume-----%d\n", nArr[i]);
		nArr[i] = 0;
		sem_post(&consumer_num);
		i = (i + 1) % NUM;
		sleep(1);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t p_tid, c_tid;
	int ret = 0;

	sem_init(&producer_num, 0, 0);
	sem_init(&consumer_num, 0, 5);

	ret = pthread_create(&p_tid, NULL, producer, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread create error : %s\n", strerror(ret));
		exit(1);
	}

	ret = pthread_create(&c_tid, NULL, producer, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread create error : %s\n", strerror(ret));
		exit(1);
	}
	
	pthread_join(p_tid, NULL);
	pthread_join(c_tid, NULL);

	sem_destroy(&producer_num);
	sem_destroy(&consumer_num);
	return 0;
}
