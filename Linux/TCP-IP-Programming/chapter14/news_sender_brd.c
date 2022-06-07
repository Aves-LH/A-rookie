#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int send_sock = 0;
	struct sockaddr_in broad_addr;
	FILE *fp = NULL;
	char buf[BUF_SIZE] = "";
	int so_brd = 1;

	if(3 != argc)
	{
		printf("Usage: %s <Boradcast IP> <port>\n", *argv);
		exit(1);
	}

	send_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == send_sock)
	{
		error_handling("socket() error!");
	}

	memset(&broad_addr, 0, sizeof(broad_addr));
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
	broad_addr.sin_port = htons(atoi(argv[2]));

	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void *)&so_brd, sizeof(so_brd));

	if(NULL == (fp = fopen("news.txt", "r")))
	{
		error_handling("fopen() error!");
	}

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&broad_addr, sizeof(broad_addr));
		sleep(2);
	}

	close(send_sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
