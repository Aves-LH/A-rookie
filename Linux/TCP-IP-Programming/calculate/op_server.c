#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);
int calculate(char op, int opnum, int *ops);

int main(int argc, char* argv[])
{
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	char message[1024] = "";
	socklen_t clnt_addr_len;
	char *ptemp = NULL;
	char op = '\0';
	int result = 0;
	int recv_len = 0;
	int opnd_cnt = 0;

	/*if(2 != argc)
	{
		printf("Usage: %s\n", argv[0]);
		exit(1);
	}*/

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("9090"));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("bind() error!");
	}

	if(-1 == listen(serv_sock, 5))
	{
		error_handling("listen() error!");
	}

	clnt_addr_len = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
	if(-1 == clnt_sock)
	{
		error_handling("accept() error!");
	}

	while(1)
	{
		recv_len = read(clnt_sock, message, sizeof(message) - 1);
		opnd_cnt = message[recv_len - 1];
		op = message[0];
		ptemp = message + 1;
		result = calculate(op, opnd_cnt, (int *)ptemp);
		write(clnt_sock, (char *)&result, sizeof(result));
		break;
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

int calculate(char op, int opnum, int *ops)
{
	int i = 0;
	int result = 0;
	switch(op)
	{
		case '+':
			for(i = 0; i < opnum; i++)
			{
				result += ops[i];
			}
			break;
		case '-':
			for(i = 0; i < opnum; i++)
			{
				result = (*ops);
				result -= ops[i + 1];
			}
			break;
		case '*':
			result = 1;
			for(i = 0; i < opnum; i++)
			{
				result *= ops[i];
			}
			break;
		case '/':
			for(i = 0; i < opnum; i++)
			{
				result = (*ops);
				result /= ops[i + 1];
			}
			break;
		default:
			break;
	}
	return result;
}
