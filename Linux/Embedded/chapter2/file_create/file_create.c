#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int temp = 0;

	if(2 != argc)
	{
		printf("run error!");
		return 1;
	}

	temp = creat(*(argv + 1), S_IRWXU);
	printf("file create success!\n");
	printf("file id is %d\n", temp);
		
	return 0;
}
