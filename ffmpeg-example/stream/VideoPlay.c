/*************************************************************************
	> File Name: display_SDL.c
	> 播放yuv视频文件
	> Author: 
	> Mail: 
	> Created Time: 一 11/23 16:47:14 2020
 ************************************************************************/

#include <stdio.h>
#include <SDL2/SDL.h>

const int bpp = 12;

int screen_w = 640;
int screen_h = 480;
const int pixel_w = 640;
const int pixel_h = 480;

unsigned char buffer[pixel_w * pixel_h * 3 / 2];

#define REFRESH_EVENT (SDL_USEREVENT + 1)

int thread_exit = 0;

int refresh_video(void *opaque)
{
	while(0 == thread_exit)
	{
		SDL_Event event;	//创建一个event变量
		event.type = REFRESH_EVENT;	//设置event类型为自定义类型REFRESH_EVENT
		SDL_PushEvent(&event);	//将event添加到事件队列中
		SDL_Delay(40);	//延时
	}
	return 0;
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Could not initialize SDL : %s\n", SDL_GetError());
		return -1;
	}

	SDL_Window *screen = NULL;
	screen = SDL_CreateWindow("Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(!screen)
	{
		printf("Could not create window : %s\n", SDL_GetError());
		return -1;
	}

	SDL_Renderer *sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	Uint32 pixformat = 0;
	pixformat = SDL_PIXELFORMAT_IYUV;

	SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);

	FILE *fp = fopen("output.yuv", "rb+");
	if(NULL == fp)
	{
		printf("open file error!\n");
		return -1;
	}

	SDL_Rect sdlRect;
	SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video, NULL, NULL);
	SDL_Event event;

	while(1)
	{
		SDL_WaitEvent(&event);
		if(event.type == REFRESH_EVENT)
		{
			if(fread(buffer, 1, pixel_w * pixel_h * bpp * 3 / 2, fp) != pixel_w * pixel_h * bpp / 8)
			{
				fseek(fp, 0, SEEK_SET);
				fread(buffer, 1, pixel_w * pixel_h * bpp * 3 / 2, fp);
			}

			SDL_UpdateTexture(sdlTexture, NULL, buffer, pixel_w);

			sdlRect.x = 0;
			sdlRect.y = 0;
			sdlRect.w = screen_w;
			sdlRect.h = screen_h;

			SDL_RenderClear(sdlRenderer);
			SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
			SDL_RenderPresent(sdlRenderer);
			SDL_Delay(40);
		}
		else if(event.type == SDL_WINDOWEVENT)
		{
			SDL_GetWindowSize(screen, &screen_w, &screen_h);
		}
		else if(event.type == SDL_QUIT)
		{
			break;
		}
	}

    return 0;
}
