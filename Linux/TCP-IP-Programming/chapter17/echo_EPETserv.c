#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>


#define BUF_SIZE 4
#define EPOLL_SIZE 50

void setnonblockingmode(int fd);
void error_handling(char *buf);

int main(int argc, char* argv[])
{
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t addr_size = 0;
	int str_len = 0;
	int i = 0;
	char buf[BUF_SIZE] = "";
	struct epoll_event *ep_events;
	struct epoll_event event;
	int epfd = 0;
	int event_cnt;

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		error_handling("socket() error!");
		exit(1);
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

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	setnonblockingmode(serv_sock);
	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while(1)
	{
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if(-1 == event_cnt)
		{
			error_handling("epoll_wait() error!");
			break;
		}

		puts("return epoll_wait");

		for(i = 0;i < event_cnt; i++)
		{
			if(serv_sock == ep_events[i].data.fd)
			{
				addr_size = sizeof(clnt_addr);
				clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
				if(-1 == clnt_sock)
				{
					error_handling("accept() error!");
				}

				setnonblockingmode(clnt_sock);
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("Connected client: %d\n", clnt_sock);
			}
			else
			{
				str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
				if(0 == str_len)
				{
					epoll_ctl( epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client: %d\n", ep_events[i].data.fd);
					break;
				}
				else if(str_len < 0)
				{
					if(EAGAIN == errno)
					{
						break;
					}
				}
				else
				{
					write(ep_events[i].data.fd, buf, str_len);
				}
			}
		}
	}

	close(serv_sock);
	close(epfd);

	return 0;
}


void setnonblockingmode(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
