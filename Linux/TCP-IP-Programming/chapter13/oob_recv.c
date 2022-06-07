#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30

void error_handling(char *message);
void urg_handler(int sig);

int acpt_sock = 0;
int recv_sock = 0;

int main(int argc, char* argv[])
{
	struct sockaddr_in recv_addr, serv_addr;
	int str_len = 0;
	int state = 0;
	socklen_t serv_addr_size = 0;
	struct sigaction act;
	char buf[BUF_SIZE] = "";
	 
	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	act.sa_handler = urg_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == acpt_sock)
	{
		error_handling("socket() error!");
	}

	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(acpt_sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr)))
	{
		error_handling("bind() error!");
	}

	if(-1 == listen(acpt_sock, 5))
	{
		error_handling("listen() error!");
	}

	serv_addr_size = sizeof(serv_addr);
	recv_sock = accept(acpt_sock, (struct sockaddr *)&serv_addr, &serv_addr_size);
	if(-1 == recv_sock)
	{
		error_handling("accept() error1");
	}

	fcntl(recv_sock, F_SETOWN, getpid());
	state = sigaction(SIGURG, &act, 0);

	while(0 != (str_len = recv(recv_sock, buf, sizeof(buf), 0)))
	{
		if(-1 == str_len)
		{
			continue;
		}
		buf[str_len] = 0;
		puts(buf);
	}
	close(recv_sock);
	close(acpt_sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void urg_handler(int sig)
{
	int str_len = 0;
	char buf[BUF_SIZE] = "";
	str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
	buf[str_len] = 0;
	printf("Urgent message : %s \n", buf);
}
