/*************************************************************************
	> File Name: pthread_detach_attr.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月20日 星期六 09时28分51秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* tfn(void *arg)
{
	printf("I am child pid: %d, tid : %lu\n", getpid(), pthread_self());
	return NULL;
}

int main(int argc, char* argv[])
{
	int ret = 0;
	pthread_t tid;
	pthread_attr_t attr;
	void *retval = NULL;

	ret = pthread_attr_init(&attr);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_attr_init error : %s\n", strerror(ret));
		exit(1);
	}

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_attr_seetdetachstate error : %s\n",strerror(ret));
		exit(1);
	}

	pthread_create(&tid, &attr, tfn, NULL);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
		exit(1);
	}
	
	ret = pthread_join(tid, &retval);
	if(0 != ret)
	{
		fprintf(stderr, "pthread_join error : %s\n", strerror(ret));
		exit(1);
	}

	pthread_attr_destroy(&attr);

	pthread_exit((void *)0);
}
