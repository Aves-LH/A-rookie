#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int acpt_sock = 0;
	int recv_sock = 0;
	struct sockaddr_in acpt_addr, recv_addr;
	int str_len = 0;
	int state = 0;
	socklen_t recv_addr_size = 0;
	char buf[BUF_SIZE] = "";

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == acpt_sock)
	{
		error_handling("socket() error!");
	}

	memset(&acpt_addr, 0, sizeof(acpt_addr));
	acpt_addr.sin_family = AF_INET;
	acpt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	acpt_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(acpt_sock, (struct sockaddr *)&acpt_addr, sizeof(acpt_addr)))
	{
		error_handling("bind() error!");
	}

	if(-1 == listen(acpt_sock, 5))
	{
		error_handling("listen() error!");
	}

	recv_addr_size = sizeof(recv_addr);
	recv_sock = accept(acpt_sock, (struct sockaddr *)&recv_addr, &recv_addr_size);

	while(1)
	{
		str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
		if(strlen > 0)
		{
			break;
		}
	}

	buf[str_len] = '\0';
	printf("Buffering %d bytes : %s\n", str_len, buf);

	str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
	buf[str_len] = 0;

	printf("Read again: %s\n", buf);
	close(acpt_sock);
	close(recv_sock);


	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
