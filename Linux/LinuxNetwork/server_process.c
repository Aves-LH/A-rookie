/*************************************************************************
	> File Name: server_process.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月23日 星期二 15时02分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>


void catch_child(int signo)
{
	int wpid = 0;
	int status = 0;

	while(-1 != (wpid = waitpid(0, &status, WNOHANG)))
	{
		/*if(WIFEXITED(status))
		{
			printf("%d disconnected!\n", wpid);
		}*/
	}
	return;
}

int main(int argc, char* argv[])
{
	sigset_t set, oldset;
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size = 0;
	pid_t pid;
	char message[BUFSIZ] = "";

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", *argv);
		exit(1);
	}

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	if(-1 == sigprocmask(SIG_BLOCK,&set, &oldset))
	{
		perror("sigprocmask error");
		exit(1);
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == serv_sock)
	{
		perror("socket error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		perror("bind error");
		exit(1);
	}

	if(-1 == listen(serv_sock, 5))
	{
		perror("listen error");
		exit(1);
	}

	clnt_addr_size = sizeof(clnt_addr);
	while(1)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if(-1 == clnt_sock)
		{
			continue;
		}
		
		pid = fork();
		if(0 == pid)
		{
			printf("%d client connected...\n", getpid());
			close(serv_sock);
			while(1)
			{
				int str_len = read(clnt_sock, message, BUFSIZ);
				if(-1 == str_len)
				{
					perror("read error");
					exit(1);
				}
				else if(0 == str_len)
				{
					printf("%d disconnected!\n", getpid());
					exit(1);
				}
	
				for(int i = 0; '\0' != message[i]; i++)
				{
					message[i] -= 32;
				}
				write(clnt_sock, message, str_len);
			}
			close(clnt_sock);
		}	
		else if(pid > 0)
		{
			close(clnt_sock);
			struct sigaction sigact;

			sigact.sa_handler = catch_child;
			sigemptyset(&sigact.sa_mask);
			sigact.sa_flags = 0;

			if(-1 == sigaction(SIGCHLD, &sigact, NULL))
			{
				perror("sigaction error");
				exit(1);
			}

			if(-1 == sigprocmask(SIG_UNBLOCK, &set, &oldset))
			{
				perror("sigprocmask error");
				exit(1);
			}
		}
		else if(-1 == pid)
		{
			perror("fork error");
			exit(1);
		}
	}
	close(serv_sock);

	return 0;
}
