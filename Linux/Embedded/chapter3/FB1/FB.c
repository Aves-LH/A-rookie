#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


int w = 0;
int h = 0;
int bpp = 0;
short *fbmem = NULL;


void fb_point(int x, int y, short color)
{
	fbmem[y * w + x] = color;
}

void fb_fill(int x1, int x2, int y1, int y2, short color)
{
	int i = 0;
	int j = 0;

	for(j = y1; j <= y2; j++)
	{
		for(i = x1; i <= x2; i++)
		{
			fb_point(i, j, color);
			usleep(5000);
		}
	}
}

void fb_move()
{
	int i = 0;
	for(i = 0; i <= 800; i++)
	{
		fb_fill(i, 0, i + 100, 100, 0x1f);
		usleep(5000);
		fb_fill(i, 0, i + 100, 100, 0x0);
	}
}

int main(int argc, char* argv[])
{
	int fb = 0;
	//int i, j = 0;
	struct fb_var_screeninfo fb_var;
	fb = open("/dev/fb0", O_RDWR);

	if(fb < 0)
	{
		printf("Error!\n");
	}
	else
	{
		printf("fb = %d\n", fb);
	}

	ioctl(fb, FBIOGET_VSCREENINFO, &fb_var);
	w = fb_var.xres;
	h = fb_var.yres;
	bpp = fb_var.bits_per_pixel;

	printf("w = %d\nh = %d\nbpp = %d\n", w, h, bpp);
	
	fbmem = mmap(0, w * h * bpp / 8, PROT_WRITE | PROT_READ, MAP_SHARED, fb, 0);

	fb_fill(100, 100, 200, 300, 0x1f);

	//fb_move();

	close(fb);

	return 0;
}
