#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char message[30] = "";
	int str_len = 0;

	if(argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect() error");
	}

	while(1)
	{
		printf("Message: ");
		scanf("%s", message);
		if(!strcmp(message, "exit"))
		{
			break;
		}
		str_len = write(sock, message, sizeof(message));
		if(-1 == str_len)
		{
			perror("write error");
			exit(1);
		}

		str_len = read(sock, message, 30);
		if(-1 == str_len)
		{
			perror("read error");
			exit(1);
		}
		message[str_len] = '\0';
		printf("Message from Server : %s\n", message);
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
