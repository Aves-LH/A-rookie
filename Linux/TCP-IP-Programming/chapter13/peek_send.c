#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock = 0;
	struct sockaddr_in send_addr;
	if(3 != argc)
	{
		printf("Usage: %s <IP> <port>\n", *argv);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error!");
	}

	memset(&send_addr, 0, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = inet_addr(argv[1]);
	send_addr.sin_port = htons(atoi(argv[2]));

	if(-1 == connect(sock, (struct sockaddr *)&send_addr, sizeof(send_addr)))
	{
		error_handling("connect() error!");
	}

	write(sock, "123", 3);
	close(sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
