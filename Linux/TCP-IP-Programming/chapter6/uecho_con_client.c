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
	int str_len = 0;
	socklen_t addr_size = 0;
	struct sockaddr_in serv_addr, from_addr;

	if(3 != argc)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error!");
	}	

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	while(1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}

		sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		addr_size = sizeof(from_addr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &addr_size);
		message[str_len] = 0;
		printf("Message from server: %s", message);
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