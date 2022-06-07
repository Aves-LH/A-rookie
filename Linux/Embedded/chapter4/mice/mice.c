#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define C_W 10
#define C_H 17
#define T___   0X0
#define BORD   0x0
#define X___   0xFFFF

static short cursor_pixel[C_W * C_H] = {
   BORD, T___, T___, T___, T___, T___, T___, T___, T___, T___,
   BORD, BORD, T___, T___, T___, T___, T___, T___, T___, T___,
   BORD, X___, BORD, T___, T___, T___, T___, T___, T___, T___,
   BORD, X___, X___, BORD, T___, T___, T___, T___, T___, T___,
   BORD, X___, X___, X___, BORD, T___, T___, T___, T___, T___,
   BORD, X___, X___, X___, X___, BORD, T___, T___, T___, T___,
   BORD, X___, X___, X___, X___, X___, BORD, T___, T___, T___,
   BORD, X___, X___, X___, X___, X___, X___, BORD, T___, T___,
   BORD, X___, X___, X___, X___, X___, X___, X___, BORD, T___,
   BORD, X___, X___, X___, X___, X___, X___, X___, X___, BORD,
   BORD, X___, X___, X___, X___, X___, BORD, BORD, BORD, BORD,
   BORD, X___, X___, BORD, X___, X___, BORD, T___, T___, T___,
   BORD, X___, BORD, T___, BORD, X___, X___, BORD, T___, T___,
   BORD, BORD, T___, T___, BORD, X___, X___, BORD, T___, T___,
   T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
   T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
   T___, T___, T___, T___, T___, T___, BORD, BORD, T___, T___
};

typedef struct 
{
	int dx;
	int dy;
	int dz;
	int button;
}mevent_t;

typedef struct
{
	int fbfd;
	struct fb_var_screeninfo fb_var;
	int w;
	int h;
	int bpp;
	short *fbmem;
}fbdev, *pfbdev;

int fb_init(pfbdev pfb);
int mouse(int mfd, mevent_t mevent);
void fb_point(fbdev fb_info, int x, int y, short color);
void mouse_draw(fbdev fb_info, int x, int y);

int main(int argc, char* argv[])
{
	fbdev fb_info;
	int mfd = 0;
	mevent_t mevent;
	char buf[] = {1,0,0,0};
	mfd = open("/dev/input/mice", O_RDWR);

	if(-1 == mfd)
	{
		printf("open file error!\n");
	}
	
	fb_init(&fb_info);

	write(mfd, buf, 4);
	while(1)
	{
		mouse(mfd, mevent);
		mouse_draw(fb_info, 400, 400);
	}
	

	
	close(mfd);

	return 0;
}

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

int fb_init(pfbdev pfb)
{
	pfb->fbfd = open("/dev/fb0", O_RDWR);

	if(pfb->fbfd < 0)
	{
		printf("Error!\n");
	}
	else
	{
		printf("fb = %d\n", pfb->fbfd);
	}

	ioctl(pfb->fbfd, FBIOGET_VSCREENINFO, &(pfb->fb_var));
	pfb->w = pfb->fb_var.xres;
	pfb->h = pfb->fb_var.yres;
	pfb->bpp = pfb->fb_var.bits_per_pixel;

	printf("w = %d\nh = %d\nbpp = %d\n", pfb->w, pfb->h, pfb->bpp);
	
	pfb->fbmem = mmap(0, (pfb->w) * (pfb->h) * (pfb->bpp) / 8, PROT_WRITE | PROT_READ, MAP_SHARED, pfb->fbfd, 0);

	

	return 0;
}

void fb_point(fbdev fb_info, int x, int y, short color)
{
	fb_info.fbmem[y * fb_info.w + x] = color;
}

void mouse_draw(fbdev fb_info, int x, int y)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < C_W; i++)
	{
		for(j = 0; j < C_H; j++)
		{
			fb_point(fb_info, x + i, y + j, cursor_pixel[j * C_W + i]);
		}
	}
}
