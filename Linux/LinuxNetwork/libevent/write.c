/*************************************************************************
	> File Name: write.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月01日 星期三 07时48分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <event2/event.h>

void write_cb(evutil_socket_t fd, short what, void *arg)
{
	char message[BUFSIZ] = "";
	sprintf(message, "Hello World!");
	write(fd, message, BUFSIZ);
}

int main(int argc, char* argv[])
{
	unlink("myfifo");

	mkfifo("myfifo", 0644);
	int fd = open("myfifo", O_CREAT | O_WRONLY, 0644);
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

	struct event *ev = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, NULL);

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
