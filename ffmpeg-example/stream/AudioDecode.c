/*************************************************************************
	> File Name: AudioDecode.c
	> 将mp3文件解码为pcm文件
	> Author: 
	> Mail: 
	> Created Time: 二 11/24 13:39:02 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

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
	char url[] = "qwe.mp3";

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

	int audioStream = -1;
	for(int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codec->codec_type)
		{
			audioStream = i;
			break;
		}
	}

	if(-1 == audioStream)
	{
		printf("Didn't find a audio stream!\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[audioStream]->codec;
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

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);

	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = pCodecCtx->frame_size;
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = 44100;
	//获取声道数
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	//获取音频文件需要的缓存大小
	int out_buf_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
	out_buf = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	pFrame = av_frame_alloc();
	//为给定的声道数获取默认声道布局
	in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);

	au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate, in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);

	while(av_read_frame(pFormatCtx, packet) >= 0)
	{
		if(packet->stream_index == audioStream)
		{
			ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_pic, packet);
			if(ret < 0)
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
		}
		av_free_packet(packet);
	}

	swr_free(&au_convert_ctx);
	fclose(pFile);

	av_free(out_buf);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
    return 0;
}
