#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char* argv[])
{
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr, clnt_addr;
	int fds[2] = {};
	pid_t pid = 0;
	struct sigaction act;
	socklen_t addr_size = 0;
	int str_len = 0;
	int state = 0;
	char buf[BUF_SIZE] = "";

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("bind() error!");
	}

	if(-1 == listen(serv_sock, -1))
	{
		error_handling("listen() error!");
	}

	pipe(fds);
	pid = fork();
	if(0 == pid)
	{
		FILE *fp = fopen("echomsg.txt", "wt");
		char msgbuf[BUF_SIZE] = "";
		int i = 0;
		int len = 0;

		for(i = 0; i < 10; i++)
		{
			len = read(fds[0], msgbuf, BUF_SIZE);
			fwrite((void *)msgbuf, 1, len, fp);
		}
		fclose(fp);
		return 0;
	}
	
	while(1)
	{
		addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
		if(-1 == clnt_sock)
		{
			continue;
		}
		else
		{
			printf("new client connected...");
		}

		pid = fork();
		if(0 == pid)
		{
			close(serv_sock);
			while(0 != (str_len = read(clnt_sock, buf, BUF_SIZE)))
			{
				write(clnt_sock, buf, str_len);
				write(fds[1], buf, str_len);
			}
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
		{
			close(clnt_sock);
		}
	}
	close(serv_sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig)
{
	pid_t pid = 0;
	int status = 0;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d\n", pid);
}

