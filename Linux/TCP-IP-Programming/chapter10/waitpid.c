#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status = 0;
	pid_t pid = fork();

	if(0 == pid)
	{
		sleep(15);
		return 24;
	}
	else
	{
		sleep(3);
		while(!waitpid(-1, &status, WNOHANG))
		{
			sleep(3);
			puts("sleep 3sec!");
		}

		if(WIFEXITED(status))
		{
			printf("Child send %d\n", WEXITSTATUS(status));
		}
	}

	return 0;
}
