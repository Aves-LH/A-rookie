#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int temp1, temp2, temp3, temp4;

	temp1 = open("./test1", O_RDWR | O_CREAT, S_IRWXU);
	temp2 = open("./test2", O_RDWR | O_CREAT, 0700);
	temp3 = open("./test3", O_RDWR | O_CREAT, 00700);
	temp4 = open("./test4", O_RDWR | O_CREAT, 700);

	printf("temp1 = %d\n", temp1);

	printf("temp2 = %d\n", temp2);

	printf("temp3 = %d\n", temp3);

	printf("temp4 = %d\n", temp4);

	temp1 = close(temp1);
	temp2 = close(temp2);
	temp3 = close(temp3);
	temp4 = close(temp4);
	
	printf("temp1 = %d\n", temp1);	
	return 0;
}
