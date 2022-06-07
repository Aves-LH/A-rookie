#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds[2] = {};
	char str[] = "Who are you?";
	char buf[BUF_SIZE] = "";
	pid_t pid = 0;

	pipe(fds);
	pid = fork();
	if(0 == pid)
	{
		printf("I am a child process.\n");
		write(fds[1], str, sizeof(str));
	}
	else
	{
		printf("Child process ID: %d\n", pid);
		printf("I am a parent process!\n");
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}


	return 0;
}
