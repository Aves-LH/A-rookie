#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int send_sock = 0;
	struct sockaddr_in mul_addr;
	int time_live = TTL;
	FILE *fp = NULL;
	char buf[BUF_SIZE] = "";

	if(3 != argc)
	{
		printf("Usage: %s <GroupIP> <port>\n", *argv);
		exit(1);
	}

	send_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == send_sock)
	{
		error_handling("socket() error!");
	}

	memset(&mul_addr, 0, sizeof(mul_addr));
	mul_addr.sin_family = AF_INET;
	mul_addr.sin_addr.s_addr = inet_addr(argv[1]);
	mul_addr.sin_port = htons(atoi(argv[2]));

	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live));

	if(NULL == (fp = fopen("news.txt", "r")))
	{
		error_handling("fopen() error!");
	}

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_addr, sizeof(mul_addr));
		sleep(2);
	}

	fclose(fp);
	close(send_sock);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
