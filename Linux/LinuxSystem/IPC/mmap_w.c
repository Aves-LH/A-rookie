/*************************************************************************
	> File Name: mmap_w.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月16日 星期二 14时45分51秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

struct STUDENT
{
	int id;
	char szName[20];
	int age;
};

int main(int argc, char* argv[])
{
	int fd;
	struct STUDENT stu = {1, "Jack", 18};
	struct STUDENT *pStu = NULL;

	fd = open("mmap.data", O_RDWR);
	if(-1 == fd)
	{
		perror("open error!");
		exit(1);
	}

	ftruncate(fd, sizeof(stu));

	pStu = (struct STUDENT *)mmap(NULL, sizeof(struct STUDENT), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(pStu == MAP_FAILED)
	{
		perror("mmap error!");
		exit(1);
	}
	close(fd);

	while(1)
	{
		memcpy(pStu, &stu, sizeof(struct STUDENT));
		stu.id++;
		sleep(1);
	}

	munmap(pStu, 0);

	return 0;
}
