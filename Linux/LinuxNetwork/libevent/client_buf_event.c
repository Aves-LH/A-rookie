/*************************************************************************
	> File Name: client_buf_event.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月02日 星期四 09时24分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/event.h>


void read_cb(struct bufferevent *bev, void *ctx)
{
	char buf[BUFSIZ] = "";
	int str_len = 0;

	bufferevent_read(bev, buf, BUFSIZ);
	printf("message from server : %s\n", buf);
	bufferevent_write(bev, buf, str_len);
	return;
}

void write_cb(struct bufferevent *bev, void *ctx)
{
	printf("send data succesfully!\n");
	return;
}

void event_cb(struct bufferevent *bev,short events, void *ctx)
{
	if(events & BEV_EVENT_EOF)
	{
		printf("connection closed!\n");
	}	
	else if(events & BEV_EVENT_ERROR)
	{
		printf("error!\n");
	}
	else if(events & BEV_EVENT_CONNECTED)
	{
		printf("connected successfully!\n");
		return;
	}
	bufferevent_free(bev);
}

void read_terminal(evutil_socket_t fd, short what, void *arg)
{
	char buf[BUFSIZ] = "";
	int str_len = 0;

	str_len = read(fd, buf, BUFSIZ);

	int ret = bufferevent_write((struct bufferevent *)arg, buf, str_len);
	return;
}

int main(int argc, char* argv[])
{
	int fd;
	struct sockaddr_in serv_addr;
	struct event_base *base = NULL;
	struct bufferevent *bev = NULL;

	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd)
	{
		perror("socket error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	base = event_base_new();
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_socket_connect(bev, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);

	bufferevent_enable(bev, EV_READ);
	bufferevent_enable(bev, EV_WRITE);

	struct event *ev = event_new(base , STDIN_FILENO, EV_READ | EV_PERSIST, read_terminal, bev);

	event_add(ev, NULL);

	event_base_dispatch(base);

	event_free(ev);
	event_base_free(base);

	close(fd);
	

	return 0;
}
