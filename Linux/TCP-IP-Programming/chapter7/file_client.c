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
	int sd = 0;
	FILE *fp = NULL;
	char buf[BUF_SIZE] = "";
	int read_cnt = 0;
	struct sockaddr_in serv_addr;
	
	if(3 != argc)
	{
		printf("Usage: %s <IP> <port>\n", *argv);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sd)
	{
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(-1 == connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		error_handling("connect() error!");
	}

	while(0 != (read_cnt = read(sd, buf, BUF_SIZE)))
	{
		fwrite((void *)buf, 1, read_cnt, fp);
	}

	puts("Received file data!");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);


	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
