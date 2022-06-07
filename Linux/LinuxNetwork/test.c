/*************************************************************************
	> File Name: test.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月27日 星期六 16时54分48秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

int main(int argc, char* argv[])
{
	int lfd, cfd;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	int i, j = 0;
	int str_len = 0;
	int maxfd = 0;
	int nready = 0;
	char message[BUFSIZ] = "";

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", *argv);
		exit(1);
	}

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == lfd)
	{
		perror("socket error");
		exit(1);
	}

	int opt = 1;
	if(-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)))
	{
		perror("setsockopt error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		perror("bind error");
		exit(1);
	}

	if(-1 == listen(lfd, 5))
	{
		perror("listen error");
		exit(1);
	}

	fd_set rset, allset;

	FD_ZERO(&allset);
	FD_SET(lfd, &allset);
	maxfd = lfd;

	while(1)
	{
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if(-1 == nready)
		{
			perror("select error");
			exit(1);
		}	

		if(FD_ISSET(lfd, &rset))
		{
			clnt_addr_size = sizeof(clnt_addr);
			cfd = accept(lfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
			if(-1 == cfd)
			{
				perror("accept error");
				exit(1);
			}
			printf("%d client connected!\n", cfd);
				
			FD_SET(cfd, &allset);

			if(maxfd < cfd)
			{
				maxfd = cfd;
			}

			if(0 == --lfd)
			{
				continue;
			}
		}

		for(i = lfd+1; i <= maxfd; i++)
		{
			if(FD_ISSET(i, &rset))
			{
				str_len = read(i, message, BUFSIZ);
				if(0 == str_len)
				{
					printf("%d client disconnectded!\n", i);
					FD_CLR(i, &allset);
					close(i);
				}
				
				for(j = 0; j < str_len; j++)
				{
					message[j] -= 32;
				}

				write(i, message, str_len);
			}
		}
	}

	return 0;
}
