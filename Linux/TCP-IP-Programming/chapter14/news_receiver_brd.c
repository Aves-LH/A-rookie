#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int recv_sock = 0;
	struct sockaddr_in addr;
	int str_len = 0;
	char buf[BUF_SIZE] = "";

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	recv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == recv_sock)
	{
		error_handling("socket() error!");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr)))
	{
		error_handling("bind() error!");
	}

	while(1)
	{
		str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if(str_len < 0)
		{
			break;
		}
		buf[str_len] = 0;
		fputs(buf, stdout);
	}
	close(recv_sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
