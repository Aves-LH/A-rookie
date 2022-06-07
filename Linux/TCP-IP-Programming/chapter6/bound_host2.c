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
	char msg1[] = "Hi!";
	char msg2[] = "I am another UDP host!";
	char msg3[] = "Nice to meet you!";

	struct sockaddr_in your_addr;
	socklen_t your_addr_size;

	if(3 != argc)
	{
		printf("Usage: %s <IP> <port>\n", *argv);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error!");
	}

	memset(&your_addr, 0, sizeof(your_addr));
	your_addr.sin_family = AF_INET;
	your_addr.sin_addr.s_addr = inet_addr(argv[1]);
	your_addr.sin_port = htons(atoi(argv[2]));

	sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&your_addr, sizeof(your_addr));

	sendto(sock, msg1, sizeof(msg2), 0, (struct sockaddr *)&your_addr, sizeof(your_addr));

	sendto(sock, msg1, sizeof(msg3), 0, (struct sockaddr *)&your_addr, sizeof(your_addr));
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
