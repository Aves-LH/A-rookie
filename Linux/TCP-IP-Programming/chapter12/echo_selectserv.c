#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);


int main(int argc, char* argv[])
{
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	struct timeval timeout;
	fd_set reads, cpy_reads;
	socklen_t addr_size = 0;
	int fd_max = 0;
	int str_len = 0;
	int fd_num = 0;
	int i = 0;
	char buf[BUF_SIZE] = "";

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", *argv);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("bind() error!");
	}

	if(-1 == listen(serv_sock, 5))
	{
		error_handling("listen() error!");
	}

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while(1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if(-1 == (fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)))
		{
			break;
		}
		if(0 == fd_num)
		{
			continue;
		}

		for(i = 0; i < fd_max + 1; i++)
		{
			if(FD_ISSET(i, &cpy_reads))
			{
				if(i == serv_sock)
				{
					addr_size = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
					if(-1 == clnt_sock)
					{
						error_handling("accept() error!");
					}
					FD_SET(clnt_sock, &reads);
					if(fd_max < clnt_sock)
					{
						fd_max = clnt_sock;
					}
					printf("connected client : %d\n", clnt_sock);
				}
				else
				{
					str_len = read(i , buf, BUF_SIZE);
					if(0 == str_len)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client : %d\n", i);
					}
					else
					{
						write(i, buf, str_len);
					}
				}
			}
		}
	}


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
