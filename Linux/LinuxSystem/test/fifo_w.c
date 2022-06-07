/*************************************************************************
	> File Name: fifo.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月05日 星期日 20时41分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	
	//write process
	int fd = open("f1", O_WRONLY);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	sleep(2);
	int i = 0;
	char buf[50] = "Hello World!";
	while(1)
	{
		sprintf(buf, "Hello World---%d\n", i);
		write(fd, buf, sizeof(buf));
		i++;
		sleep(1);
	}	

	return 0;
}
