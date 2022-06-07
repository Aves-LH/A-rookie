#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int argc, char* argv[])
{
	pid_t pid = 0;
	int lval = 20;
	gval++, lval+=5;

	pid = fork();
	if(0 == pid)
	{
		gval += 2;
		lval += 2;
	}
	else
	{
		gval -= 2;
		lval -= 2;
	}

	if(0 == pid)
	{
		printf("Child Proc: [%d, %d]\n", gval, lval);
		sleep(2);
	}
	else
	{
		printf("Parent Proc: [%d, %d]\n", gval, lval);
	}

	return 0;
}
