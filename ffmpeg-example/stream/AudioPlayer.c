/*************************************************************************
	> File Name: AudioPlayer.c
	> 拉流并只播放音频文件
	> Author: 
	> Mail: 
	> Created Time: 二 11/24 14:32:39 2020
 ************************************************************************/

#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
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

#define MAX_AUDIO_FRAME_SIZE 192000

int main(int argc, char **argv)
{
	AVFormatContext *pFormatCtx = NULL;
	AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVPacket *packet = NULL;
	AVFrame *pFrame = NULL;

	uint8_t *out_buf = NULL;
	int got_pic = 0;
	int index = 0;
	int64_t in_channel_layout = 0;
	struct SwrContext *au_convert_ctx = NULL;

	FILE *pFile = fopen("output.pcm", "wb+");
	//char url[] = "PartyAnimal.mp4";
	char url[] = "rtmp://localhost:1935/live/room";

	av_register_all();
	avformat_network_init();

	pFormatCtx = avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx, url, NULL, NULL) < 0)
	{
		printf("Could not open input stream!\n");
		return -1;
	}

	if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Could not find stream information!\n");
		return -1;
	}

	av_dump_format(pFormatCtx, 0, url, 0);

	int audioindex = -1;
	for(int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codec->codec_type)
		{
			audioindex = i;
			break;
		}
	}

	if(-1 == audioindex)
	{
		printf("Didn't find a audio stream!\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[audioindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(NULL == pCodec)
	{
		printf("Codec not found!\n");
		return -1;
	}

	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec!\n");
		return -1;
	}

	pFrame = av_frame_alloc();
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);

	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = pCodecCtx->frame_size;
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = 44100;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	int out_buf_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
	out_buf = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

	in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
	au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate, in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);

	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("Could not initialize SDL : %s\n", SDL_GetError());
		return -1;
	}

	//创建SDL_AudioSpec，设置音频播放参数
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = out_sample_rate;		//采样率
	wanted_spec.format = AUDIO_S16SYS;		//采样格式
	wanted_spec.channels = out_channels;	//声道数
	wanted_spec.silence = 0;				//无输出时是否静音
	wanted_spec.samples = out_nb_samples;	//默认每次读音频缓存的大小
	wanted_spec.callback = fill_audio;		//设置读取音频数据的回调函数
	wanted_spec.userdata = pCodecCtx;		//用户数据

	if(SDL_OpenAudio(&wanted_spec, NULL) < 0)
	{
		printf("Could not open audio!\n");
		return -1;
	}

	SDL_PauseAudio(0);

	while(av_read_frame(pFormatCtx, packet) >= 0)
	{
		if(packet->stream_index == audioindex)
		{
			//对音频数据进行解码，解码完成后会再次回到音频处理回调函数
			if(avcodec_decode_audio4(pCodecCtx, pFrame, &got_pic, packet) < 0)
			{
				printf("Error in decoding audio frame!\n");
				return -1;
			}

			if(got_pic > 0)
			{
				swr_convert(au_convert_ctx, &out_buf, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data, pFrame->nb_samples);
				printf("index : %5d\t pts : %lld\t packet size : %d\n", index, packet->pts, packet->size);
				fwrite(out_buf, 1, out_buf_size, pFile);
				index++;
			}

			while(audio_len > 0)
			{
				SDL_Delay(1);
			}
			audio_chunk = (uint8_t *)out_buf;
			audio_len = out_buf_size;
			audio_pos = audio_chunk;	
		}
		av_free_packet(packet);
	}

	swr_free(&au_convert_ctx);
	fclose(pFile);

	av_free(out_buf);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	SDL_CloseAudio();
	SDL_Quit();

    return 0;
}
