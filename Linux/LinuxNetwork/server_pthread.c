/*************************************************************************
	> File Name: server_pthread.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月24日 星期三 14时39分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void* msg_handler(void *arg)
{
	int clnt_sock = (int)arg;
	int str_len = 0;
	char msg[BUFSIZ] = "";
	int i = 0;

	while(1)
	{
		str_len = read(clnt_sock, msg, BUFSIZ);
		if(0 == str_len)
		{
			break;
		}	

		for(i = 0; '\0' != msg[i]; i++)
			msg[i] -= 32;
		
		write(clnt_sock, msg, str_len);
	}
	printf("%d client disconnected!\n", clnt_sock);
	close(clnt_sock);

	return NULL;
}

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr[1024];
	socklen_t clnt_addr_size;
	pthread_t tid;
	int ret = 0;
	int i = 0;
	int opt = 1;

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock  = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		perror("socket error");
		exit(1);
	}

	ret = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	if(-1 == ret)
	{
		perror("setsockopt error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		perror("bind error");
		exit(1);
	}

	if(-1 == listen(serv_sock, 5))
	{
		perror("listen error");
		exit(1);
	}

	clnt_addr_size = sizeof(clnt_addr);
	while(1)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr[i], &clnt_addr_size);
		if(-1 == clnt_sock)
		{
			continue;
		}

		printf("%d client connected...\n", clnt_sock);

		ret = pthread_create(&tid, NULL, msg_handler, (void *)clnt_sock);
		if(0 != ret)
		{
			fprintf(stderr, "pthread create error : %s\n", strerror(ret));
			exit(1);
		}

		ret = pthread_detach(tid);
		if(0 != ret)
		{
			fprintf(stderr, "pthread create error : %s\n", strerror(ret));
			exit(1);
		}
		i++;
	}
	close(serv_sock);

	return 0;
}
