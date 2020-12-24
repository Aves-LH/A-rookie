/*************************************************************************
	> File Name: AudioPlayer.c
	> Author: 
	> Mail: 
	> Created Time: 二 11/24 14:16:37 2020
 ************************************************************************/

#include <stdio.h>
#include <SDL2/SDL.h>

static uint8_t *audio_chunk = NULL;
static uint32_t audio_len = 0;
static uint8_t *audio_pos = NULL;

void fill_audio(void *udata, uint8_t *stream, int len)
{
	SDL_memset(stream, 0, len);
	if(0 == audio_len)
	{
		return;
	}

	len = (len > audio_len ? audio_len : len);
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("Could not initialize SDL : %s\n", SDL_GetError());
		return -1;
	}

	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = 44100;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = 2;
	wanted_spec.silence = 0;
	wanted_spec.samples = 1024;
	wanted_spec.callback = fill_audio;

	if(SDL_OpenAudio(&wanted_spec, NULL) < 0)
	{
		printf("Could not open audio!\n");
		return -1;
	}

	FILE *fp = fopen("output.pcm", "rb+");
	if(NULL == fp)
	{
		printf("open file error!\n");
		return -1;
	}

	int pcm_buf_size = 4096;
	char *pcm_buf = (char *)malloc(pcm_buf_size);
	int data_count = 0;

	SDL_PauseAudio(0);

	while(1)
	{
		if(fread(pcm_buf, 1, pcm_buf_size, fp) != pcm_buf_size)
		{
			fseek(fp, 0, SEEK_SET);
			fread(pcm_buf, 1, pcm_buf_size, fp);
			data_count = 0;
		}

		printf("Now Playing %10d Bytes data!\n", data_count);
		data_count += pcm_buf_size;
		audio_chunk = (uint8_t *)pcm_buf;
		audio_len = pcm_buf_size;
		audio_pos = audio_chunk;

		while(audio_len > 0)
		{
			SDL_Delay(1);
		}
	}

	free(pcm_buf);
	SDL_Quit();

    return 0;
}
