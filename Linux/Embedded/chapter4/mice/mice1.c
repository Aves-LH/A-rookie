#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct 
{
	int dx;
	int dy;
	int dz;
	int button;
}mevent_t;

int mouse(int mfd, mevent_t mevent)
{
	char buf[4];

	if(read(mfd, buf, 4) > 0)
	{
		mevent.button = buf[0] & 0x07;
		printf("button = %d\n", mevent.button);

		mevent.dx = buf[1];
		printf("dx = %d\n", mevent.dx);

		mevent.dy = -buf[2];
		printf("dy = %d\n", mevent.dy);

		mevent.dz = buf[3];
		printf("dz = %d\n", mevent.dz);
	}
	else
	{
		return 1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int mfd = 0;
	mevent_t mevent;
	char buf[] = {1,0,0,0};
	mfd = open("/dev/input/mice", O_RDWR);

	if(-1 == mfd)
	{
		printf("open file error!\n");
	}


	write(mfd, buf, 4);
	mouse(mfd, mevent);
	close(mfd);

	return 0;
}
