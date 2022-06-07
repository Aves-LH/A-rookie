/*************************************************************************
	> File Name: pipe.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月05日 星期日 20时21分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	int fd[2];
	int ret = 0;

	ret = pipe(fd);
	if(-1 == ret)
	{
		perror("pipe error");
		exit(1);
	}

	pid_t pid = fork();
	if(0 == pid)
	{
		close(fd[0]);
		char message[] = "Hello I am child process!\n";
		dup2(fd[1], STDOUT_FILENO);
		//execlp("ls", "ls", NULL);
		write(fd[1], message, strlen(message));
		printf("data sent successfully!\n");
		close(fd[1]);
	}
	else
	{
		sleep(2);
		close(fd[1]);
		char buf[50] = "";
		dup2(fd[0], STDIN_FILENO);
		//execlp("wc", "wc", NULL);
		read(fd[0], buf, 50);
		printf("message from child : %s\n", buf);
		close(fd[0]);
	}

	return 0;
}
