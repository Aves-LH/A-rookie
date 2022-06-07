#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int srcfilefd, tarfilefd;
	int readnum = 0;
	int nSeekNum = 0;
	char wrbuf[1024] = "";
	char szBuf[1024] = "";

	if(3 != argc)
	{
		printf("Error!\n");
		return 1;
	}

	srcfilefd = open(*(argv + 1), O_RDONLY, 0400);

	if(-1 == srcfilefd)
	{
		printf("File open error!\n");
		return 1;
	}
	
	readnum = read(srcfilefd, wrbuf, 1024);


	tarfilefd = open(*(argv + 2), O_WRONLY | O_CREAT, 0600);
	//method 1
	//tarfilefd = open(*(argv + 2), O_APPEND | O_WRONLY | O_CREAT, 0600);
	
	if(-1 == tarfilefd)
	{
		printf("File open error!\n");
		return 1;
	}

	//method 2
	lseek(tarfilefd, 0, SEEK_END);


	write(tarfilefd, wrbuf, readnum);
	close(srcfilefd);
	close(tarfilefd);

	return 0;
}
