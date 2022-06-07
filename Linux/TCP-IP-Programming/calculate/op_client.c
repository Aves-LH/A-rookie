#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char message[1024] = "";
	int result = 0;
	int opnd_cnt = 0;
	char op = '\0';
	int i = 0;
	
	/*if(3 != argc)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}*/

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock)
	{
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(atoi("9090"));

	if(-1 == connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("connect() error!");
	}
	else
	{
		printf("Connected.........\n");
	}

	printf("Operator: ");
	scanf("%c", &(message[0]));
	printf("Operand count: ");
	scanf("%d", &opnd_cnt);
	message[opnd_cnt * sizeof(int) + 1] = (char)opnd_cnt;

	for(i = 0; i < opnd_cnt; i++)
	{
		printf("Operand %d: ", i + 1);
		scanf("%d", (int *)&message[i * 4 + 1]);
	}
	
	write(sock, message, opnd_cnt * 4 + 2);
	read(sock, &result, sizeof(result));

	printf("Operation result: %d \n", result);
	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
