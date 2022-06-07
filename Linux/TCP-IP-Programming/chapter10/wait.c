#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status = 0;
	pid_t pid = fork();

	if(0 == pid)
	{
		return 3;
	}
	else
	{
		printf("Child PID: %d\n", pid);
		pid = fork();
		if(0 == pid)
		{
			exit(7);
		}
		else
		{
			printf("Child PID: %d\n", pid);
			wait(&status);
			if(WIFEXITED(status))
			{
				printf("Child send one: %d\n", WEXITSTATUS(status));
			}
			wait(&status);
			if(WIFEXITED(status))
			{
				printf("Child send two: %d\n", WEXITSTATUS(status));
			}
			sleep(30);
		}
	}


	return 0;
}
