/*************************************************************************
	> File Name: catch_child.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月18日 星期四 11时31分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

void catch_child(int signo)
{
	int wpid = 0;
	int status = 0;

	while(-1 != (wpid = waitpid(-1, &status, 0)))
	{
		if(WIFEXITED(status))
		{
			printf("----------catch child %d, retval = %d\n", wpid, WEXITSTATUS(status));
		}
	}

	return ;
}

int main(int argc, char* argv[])
{
	sigset_t set, oldset;
	pid_t pid;
	int i = 0;
	int sig_ret = 0;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sig_ret = sigprocmask(SIG_BLOCK, &set, &oldset);
	if(-1 == sig_ret)
	{
		perror("sigprocmash error!");
		exit(1);
	}

	for(i = 0; i < 15; i++)
	{
		pid = fork();
		if(-1 == pid)
		{
			perror("fork error!");
			exit(1);
		}
		else if(0 == pid)
		{
			break;
		}
	}

	if(15 == i)
	{
		struct sigaction act;

		act.sa_handler = catch_child;
		sigemptyset(&(act.sa_mask));
		act.sa_flags = 0;

		int ret = sigaction(SIGCHLD, &act, NULL);
		if(-1 == ret)
		{
			perror("sigaction error!");
			exit(1);
		}

		sig_ret = sigprocmask(SIG_UNBLOCK, &set, &oldset);
		if(-1 == sig_ret)
		{
			perror("sigprocmash error!");
			exit(1);
		}
		printf("I am parent %d\n", getpid());

		sleep(5);
		while(1);
	}
	else
	{
		sleep(1);
		printf("I am child %d\n", getpid());
		return i;
	}

	return 0;
}
