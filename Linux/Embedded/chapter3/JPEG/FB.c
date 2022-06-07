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

unsigned char* decode_jpeg(char *filename, short *widthPtr, short *heightPtr);

void fb_point(int x, int y, short color)
{
	fbmem[y * w + x] = color;
}

int rgb24to32(int color24)
{
	int color32 = 0;
	int r32, g32, b32;
	int r24, g24, b24;
	
	r24 = color24 & 0xff;
	g24 = (color24 >> 8) & 0xff;
	b24 = (color24 >> 16) & 0xff;

	r32 = r24;
	g32 = g24;
	b32 = b24;

	color32 = (r32 << 16) | (g32 << 8) | b32;

	return color32;
}

void display_jpeg(char *filename)
{
	int i = 0;
	int j = 0;
	unsigned char *buf32 = NULL;
	short imgw = 0;
	short imgh = 0;
	int color32 = 0;
	int color24 = 0;
	buf32 = decode_jpeg(filename, &imgw, &imgh);
	for(j = 0; j < imgh; i++)
	{
		for(i = 0; i < imgw; i++)
		{
			color24 = *(int *)buf32;
			color32 = rgb24to32(color24);
			fb_point(i, j, color32);
			buf32 += 4;
		}
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

	//fb_fill(100, 100, 300, 200, 0xff);
	//
	//fb_move();

	display_jpeg("original.jpg");
	close(fb);

	return 0;
}
