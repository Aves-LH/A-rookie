/*************************************************************************
	> File Name: read.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月01日 星期三 07时56分47秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <event2/event.h>

void read_cb(evutil_socket_t fd, short what, void *arg)
{
	char buf[BUFSIZ] = "";
	int str_len = 0;

	str_len = read(fd, buf, BUFSIZ);
	if(-1 == str_len)
	{
		perror("read error");
		exit(1);
	}
	
	printf("message : %s\n", buf);
	return;
}

int main(int argc, char* argv[])
{
	int fd = open("myfifo", O_RDONLY);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	struct event_base *base = event_base_new();
	if(NULL == base)
	{
		perror("event_base_new error");
		exit(1);
	}

	struct event *ev = event_new(base, fd, EV_READ | EV_PERSIST, read_cb, NULL);

	if(-1 == event_add(ev, NULL))
	{
		perror("event_add error");
		exit(1);
	}

	event_base_dispatch(base);

	event_free(ev);
	event_base_free(base);
	close(fd);

	return 0;
}
