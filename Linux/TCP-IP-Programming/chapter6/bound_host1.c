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
	int sock = 0;
	char message[BUF_SIZE] = "";
	struct sockaddr_in my_addr, your_addr;
	socklen_t addr_size;
	int str_len = 0;
	int i = 0;

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error!");
	}

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr)))
	{
		error_handling("bind() error!");
	}

	for(i = 0; i < 3; i++)
	{
		sleep(5);
		addr_size = sizeof(your_addr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&your_addr, &addr_size);
		printf("Message %d: %s\n", i + 1, message);
	}
	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
