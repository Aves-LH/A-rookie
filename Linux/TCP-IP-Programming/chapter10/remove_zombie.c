#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
	int status = 0;
	pid_t id = waitpid(-1, &status, WNOHANG);
	if(WIFEXITED(status))
	{
		printf("Removed proc id: %d\n", id);
		printf("Child send: %d\n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[])
{
	pid_t pid = 0;
	struct sigaction act;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if(0 == pid)
	{
		puts("HI, I am child process!");
		sleep(10);
		return 12;
	}
	else
	{
		sleep(3);
		printf("Child proc id: %d\n", pid);
		pid = fork();
		if(0 == pid)
		{
			puts("Hi! I am child process!");
			sleep(10);
			exit(24);
		}
		else
		{
			sleep(3);
			int i = 0;
			printf("Child proc id: %d\n", pid);
			for(i = 0; i < 5; i++)
			{
				puts("wait...");
				sleep(5);
			}
		}
	}

	return 0;
}
