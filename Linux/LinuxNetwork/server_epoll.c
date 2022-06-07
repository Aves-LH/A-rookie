/*************************************************************************
	> File Name: server_epoll.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月27日 星期六 16时28分54秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int lfd, cfd;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size = 0;
	struct epoll_event tep, ep[1024];
	int str_len = 0;
	int ret = 0;
	int i = 0;
	int j = 0;
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

	int epfd = epoll_create(1024);
	if(-1 == epfd)
	{
		perror("epoll create error");
		exit(1);
	}

	tep.events = EPOLLIN;
	tep.data.fd = lfd;

	if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &tep))
	{
		perror("epoll_ctl error");
		exit(1);
	}

	while(1)
	{
		ret = epoll_wait(epfd, ep, 1024, 0);
		if(-1 == ret)
		{
			perror("epoll_wait error");
			exit(1);
		}

		for(i = 0; i < ret; i++)
		{
			if(lfd == ep[i].data.fd)
			{
				clnt_addr_size = sizeof(clnt_addr);
				cfd = accept(lfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
				if(-1 == cfd)
				{
					perror("accept error");
					exit(1);
				}
				printf("%d client connected!\n", cfd);

				tep.events = EPOLLIN | EPOLLET;
				tep.data.fd = cfd;
				
				if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &tep))
				{
					perror("epoll_ctl error");
					exit(1);
				}
				int flags = fcntl(cfd, F_GETFL);
				flags |= O_NONBLOCK;
				fcntl(cfd, F_SETFL,flags);
			}
			else
			{
				str_len = read(ep[i].data.fd, message, BUFSIZ);
				if(0 == str_len)
				{
					printf("%d client disconnected!\n", ep[i].data.fd);
					if(-1 == epoll_ctl(epfd, EPOLL_CTL_DEL, ep[i].data.fd, NULL))
					{
						perror("epoll_ctl error");
						exit(1);
					}
					close(ep[i].data.fd);
				}
				else if(0 < str_len)
				{
					for(j = 0; '\0' != message[j]; j++)
					{
						message[j] -= 32;
					}
					write(ep[i].data.fd, message, str_len);
				}
			}
		}
	}

	return 0;
}
