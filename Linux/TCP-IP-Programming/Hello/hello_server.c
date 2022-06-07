#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock = 0;
	int clnt_sock = 0;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_size;

	char message[] = "Hello World!";
	int str_len = 0;
	char clnt_ip[20] = "";

	if(argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	if(serv_sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("bind() error");
	}

	if(-1 == listen(serv_sock, 5))
	{
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	
	printf("client IP : %s, port : %d\n", inet_ntop(AF_INET, &(clnt_addr.sin_addr.s_addr),clnt_ip, sizeof(clnt_ip)),
		       	ntohs(clnt_addr.sin_port));

	if(-1 == clnt_sock)
	{
		error_handling("accept() error");
	}

	while(1)
	{
		str_len = read(clnt_sock, message, 30);
		if(-1 == str_len)
		{
			perror("read error");
			exit(1);
		}

		int i = 0;
		for(; '\0' != message[i]; i++)
		{
			message[i] -= 32;
		}

		str_len = write(clnt_sock, message, 30);
		if(-1 == str_len)
		{
			perror("write error");
			exit(1);
		}
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
