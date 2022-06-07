/*************************************************************************
	> File Name: server_select.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月26日 星期五 14时34分41秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	int ret = 0;
	socklen_t clnt_addr_size = 0;
	int str_len = 0;
	int maxfd = 0;
	int nready = 0;

	fd_set rset, allset;
	int i = 0;
	int j = 0;
	char message[BUFSIZ] = "";

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		perror("socket error");
		exit(1);
	}

	int opt = 1;
	ret = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	if(-1 == ret)
	{
		perror("setsockopt error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		perror("bind error");
		exit(1);
	}

	if(-1 == listen(serv_sock, 128))
	{
		perror("listen error");
		exit(1);
	}

	maxfd = serv_sock;
	FD_ZERO(&allset);
	FD_SET(serv_sock, &allset);

	while(1)
	{
		rset = allset;

		nready = select(maxfd + 1, &rset, NULL, NULL, 0);
		if(0 > nready)
		{
			perror("select error");
			exit(1);
		}

		if(FD_ISSET(serv_sock, &rset))
		{
			clnt_addr_size = sizeof(clnt_addr);
			clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
			if(-1 == clnt_sock)
			{
				perror("accept error");
				exit(1);
			}	
			printf("%d client connected!\n", clnt_sock);

			FD_SET(clnt_sock, &allset);

			if(maxfd < clnt_sock)
			{
				maxfd = clnt_sock;
			}

			if(0 == --nready)
			{
				continue;
			}

		}

		for(i = serv_sock + 1; i <= maxfd; i++)
		{
			if(FD_ISSET(i, &rset))
			{
				str_len = read(i, message, sizeof(message));
				if(0 == str_len)
				{
					printf("%d client disconnected!\n", i);
					close(i);
					FD_CLR(i, &allset);
				}
				else if(-1 == str_len)
				{
					perror("read error");
					exit(1);
				}
				
				for(j = 0; '\0' != message[j]; j++)
				{
					message[j] -= 32;
				}

				message[str_len] = '\0';
				write(i, message, str_len);
			}
		}

	}

	return 0;
}
