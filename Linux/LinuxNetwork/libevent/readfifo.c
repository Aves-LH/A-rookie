/*************************************************************************
	> File Name: readfifo.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月29日 星期一 14时44分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <event2/event.h>
#include <fcntl.h>
#include <sys/stat.h>

void read_cb(evutil_socket_t fd, short what, void *arg)
{
	char buf[1024] = "";
	int n = 0;

	n = read(fd, buf, 1024);
	if(-1 == n)
	{
		perror("read error");
		exit(1);
	}

	printf("read event : %s\n", what & EV_READ ? "Yse" : "No");
	printf("Data length = %d, buf = %s\n", n, buf);
}

int main(int argc, char* argv[])
{
	unlink("myfifo");

	mkfifo("myfifo", 0644);

	int fd = open("myfifo", O_RDONLY | O_NONBLOCK);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	struct event_base *base = NULL;
	base = event_base_new();

	struct event *rd_event = event_new(base, fd, EV_READ | EV_PERSIST, read_cb, NULL);

	if(-1 == event_add(rd_event, NULL))
	{
		perror("event_add error");
		exit(1);
	}

	event_base_dispatch(base);

	event_free(rd_event);
	event_base_free(base);
	close(fd);

	return 0;
}
