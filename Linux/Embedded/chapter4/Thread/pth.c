#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void printid(char *s)
{
	pthread_t tid;
	tid = pthread_self();
	printf("%s:tid = %lu\n", s, tid);
}

void* pth_f()
{
	printid("new pthread\n");
	return NULL;
}

int main(int argc, char* argv[])
{
	int err = 0;
	pthread_t tid;
	err = pthread_create(&tid, NULL, pth_f, NULL);
	if(0 != err)
	{
		printf("Create thread error!\n");
	}
	printid("main");
	return 0;
}
