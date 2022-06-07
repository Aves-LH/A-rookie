/*************************************************************************
	> File Name: cond_producer_consumer.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月20日 星期六 16时28分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct NODE
{
	int nNum;
	struct NODE *pNext;
};

struct NODE *pHead = NULL;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* producer(void *arg)
{
	while(1)
	{
		struct NODE *pNew = (struct NODE *)malloc(sizeof(struct NODE));
		if(NULL == pNew)
		{
			return NULL;
		}
		pNew->nNum = rand() % 1000 + 1;
		pNew->pNext = NULL;
		printf("Producer-----------%d\n", pNew->nNum);

		pthread_mutex_lock(&mutex);
		pNew->pNext = pHead;
		pHead = pNew;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		sleep(3);
	}
	return NULL;

}

void* consumer(void *arg)
{
	struct NODE *pTemp = NULL;

	while(1)
	{	
		pthread_mutex_lock(&mutex);
		while(NULL == pHead)
		{
			pthread_cond_wait(&cond, &mutex);
		}

		pTemp = pHead;
		pHead = pTemp->pNext;
		pthread_mutex_unlock(&mutex);

		printf("Consumer %lu---%d\n", pthread_self(), pTemp->nNum);
		free(pTemp);
		sleep(rand() % 5);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t p_tid, c_tid1, c_tid2, c_tid3;
	int ret = 0;

	ret = pthread_mutex_init(&mutex, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "mutex init error: %s\n", strerror(ret));
		exit(1);
	}

	ret = pthread_cond_init(&cond, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "cond init error: %s\n", strerror(ret));
		exit(1);
	}

	ret = pthread_create(&p_tid, NULL, producer, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
		exit(1);
	}

	ret = pthread_create(&c_tid1, NULL, consumer, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
		exit(1);
	}
	ret = pthread_create(&c_tid2, NULL, consumer, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
		exit(1);
	}
	ret = pthread_create(&c_tid3, NULL, consumer, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
		exit(1);
	}

	pthread_join(p_tid, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_join error : %s\n", strerror(ret));
		exit(1);
	}

	pthread_join(c_tid1, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_join error : %s\n", strerror(ret));
		exit(1);
	}

	pthread_mutex_unlock(&mutex);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
