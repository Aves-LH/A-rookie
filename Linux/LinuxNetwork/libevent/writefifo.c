/*************************************************************************
	> File Name: writefifo.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月29日 星期一 14时52分59秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <event2/event.h>
#include <fcntl.h>

void write_cb(evutil_socket_t fd, short what, void *arg)
{
	char buf[1024] = "";
	int n = 1;

	sprintf(buf, "Hello World!  %d\n", n);
	n++;
	if(-1 == write(fd, buf, strlen(buf) + 1))
	{
		perror("write error");
		exit(1);
	}
	sleep(1);
}

int main(int argc, char* argv[])
{
	int fd = open("myfifo", O_WRONLY | O_NONBLOCK);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	struct event_base *base = NULL;
	base = event_base_new();

	struct event *wr_event = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, NULL);

	if(-1 == event_add(wr_event, NULL))
	{
		perror("event addr error");
		exit(1);
	}

	event_base_dispatch(base);

	event_free(wr_event);
	event_base_free(base);
	close(fd);

	return 0;
}
