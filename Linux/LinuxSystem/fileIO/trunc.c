#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int ret = truncate("news_cp.txt", 2000);

	printf("ret = %d\n", ret);
	return 0;
}
