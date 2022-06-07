#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc,char* argv[])
{
	int sock = 0;
	struct sockaddr_in recv_addr;

	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", *argv);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error!");
	}

	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	recv_addr.sin_port = htons(atoi(argv[2]));

	if(-1 == connect(sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr)))
	{
		error_handling("connect() error!");
	}

	write(sock, "123", strlen("123"));
	send(sock, "4", 1, MSG_OOB);
	write(sock, "567", 3);
	send(sock, "890", 3, MSG_OOB);
	close(sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
