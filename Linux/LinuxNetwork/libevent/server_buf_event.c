#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>


void read_cb(struct bufferevent *bev, void *ctx)
{
	char buf[BUFSIZ] = "";
	int str_len = 0;

	str_len = bufferevent_read(bev, buf, BUFSIZ);

	printf("message from client : %s\n", buf);
	bufferevent_write(bev, buf, str_len);


	return;
}

void write_cb(struct bufferevent *bev, void *ctx)
{
	printf("write data successfully!\n");
	return;
}

void event_cb(struct bufferevent *bev,short events, void *ctx)
{
	if(events & BEV_EVENT_EOF)
	{
		printf("connection closed\n");
	}
	else if(events & BEV_EVENT_ERROR)
	{
		printf("error\n");

	}
	bufferevent_free(bev);
	return;
}

void client_handler(struct evconnlistener *listener, evutil_socket_t sock, struct sockaddr *addr, int len, void *ptr)
{
	printf("new client connect!\n");
	struct bufferevent *bev;
	bev = bufferevent_socket_new((struct event_base *)ptr, sock, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
	bufferevent_enable(bev, EV_READ);

	return;
}


int main(int argc, char* argv[])
{
	struct sockaddr_in serv_addr;

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	struct event_base *base = event_base_new();
	if(NULL == base)
	{
		perror("event_base_new error");
		exit(1);
	}

	struct evconnlistener *evlistener = NULL;
	evlistener = evconnlistener_new_bind(base, client_handler,  base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 5, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	event_base_dispatch(base);
	evconnlistener_free(evlistener);
	event_base_free(base);

	return 0;
}
