#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

void* handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *msg);

int main(int argc, char* argv[])
{
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size = 0;
	pthread_t t_id;
	
	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("bind() error!");
	}

	if(-1 == listen(serv_sock ,5))
	{
		error_handling("listen() error!");
	}

	while(1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

		pthread_mutex_lock(&mutex);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutex);
		pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s\b", inet_ntoa(clnt_addr.sin_addr));
	}
	close(serv_sock);

	return 0;
}


void* handle_clnt(void *arg)
{
	int clnt_sock = *((int *)arg);
	int str_len = 0;
	int i = 0;
	char msg[BUF_SIZE] = "";

	while(0 != (str_len = read(clnt_sock, msg, sizeof(msg))))
	{
		send_msg(msg, str_len);
	}

	pthread_mutex_lock(&mutex);

	for(i = 0; i < clnt_cnt; i++)
	{
		if(clnt_sock == clnt_socks[i])
		{
			while(i++ < (clnt_cnt - 1))
			{
				clnt_socks[i]  = clnt_socks[i + 1];
			}
			break;
		}
	}

	clnt_cnt--;
	pthread_mutex_unlock(&mutex);
	close(clnt_sock);
	return NULL;
}

void send_msg(char *msg, int len)
{
	int i = 0;

	pthread_mutex_lock(&mutex);
	for(i = 0; i < clnt_cnt; i++)
	{
		write(clnt_socks[i], msg, len);
	}
	pthread_mutex_unlock(&mutex);
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
