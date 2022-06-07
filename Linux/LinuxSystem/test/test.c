/*************************************************************************
  > File Name: test.c
  > Author: amoscykl
  > Mail: amoscykl@163.com 
  > Created Time: 2020年08月11日 星期二 13时27分00秒
 ************************************************************************/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/epoll.h>

struct msg
{
	int val;
	struct msg *next;
};
struct msg *head;

pthread_mutex_t mutex;
pthread_cond_t cond;

void* producer(void *arg)
{
	while(1)
	{
		struct msg *new = (struct msg *)malloc(sizeof(struct msg));
		if(NULL != new)
		{
			new->val = rand() % 100 + 1;
			new->next = NULL;
		}

		printf("producer : %d\n", new->val);

		pthread_mutex_lock(&mutex);

		new->next = head;
		head = new;

		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		sleep(rand() % 3);
	}

	return NULL;
}

void* consumer(void *arg)
{
	struct msg *tmp = NULL;

	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(NULL == head)
		{
			pthread_cond_wait(&cond, &mutex);
		}

		tmp = head;
		head = tmp->next;
		
		pthread_mutex_unlock(&mutex);
		printf("consumer ------------- %d\n", tmp->val);
		free(tmp);
		sleep(rand() % 3);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t pid, cid;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);

	pthread_join(pid, NULL);
	pthread_join(cid, NULL);
	
	return 0;
}
