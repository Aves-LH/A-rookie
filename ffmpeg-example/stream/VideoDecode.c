/*************************************************************************
	> File Name: decode.c
	> 视频文件解码
	> Author: 
	> Mail: 
	> Created Time: 五 11/20 15:30:19 2020
 ************************************************************************/

#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>


int main(int argc, char **argv)
{
	AVFormatContext *pFormatCtx = NULL;
	AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVFrame *pFrame = NULL;
	AVFrame *pFrameYUV = NULL;
	AVPacket *packet = NULL;
	int videoindex = 0;
	unsigned char *out_buffer = NULL;
	int y_size = 0;
	int got_picture = 0;
	struct SwsContext *img_convert_ctx = NULL;
	char filepath[] = "PartyAnimal.flv";

	FILE *fp_yuv = fopen("output.yuv", "wb+");

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0)
	{
		printf("Could not open input stream!\n");
		return -1;
	}

	if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Could not find stream information!\n");
		return -1;
	}

	videoindex = -1;
	for(int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}

	if(videoindex == -1)
	{
		printf("Didn't find a video stream!\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(NULL == pCodec)
	{
		printf("Codec not found!\n");
		return -1;
	}

	//用给定的AVCodec初始化AVCodecContext
	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec!\n");
		return -1;
	}

	//分配一个AVFrame并初始化为默认值
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();
	out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
	
	//根据参数指定的图像和给定的数组设置data和linesize
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	printf("-----------------File Information-----------------\n");
	av_dump_format(pFormatCtx, 0, filepath, 0);
	printf("---------------------------------------\n");
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	while(av_read_frame(pFormatCtx, packet) >= 0)
	{
		if(packet->stream_index == videoindex)
		{
			if(avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet) < 0)
			{
				printf("Decode Error!\n");
				return -1;
			}
			
			if(got_picture)
			{
				sws_scale(img_convert_ctx, (const unsigned char * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
				y_size = pCodecCtx->width * pCodecCtx->height;
				fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
				fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
				fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);
				printf("Succeed to decode 1 frame!\n");
			}
		}
		av_free_packet(packet);
	}

	while(1)
	{
		if(avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet) < 0)
		{
			break;
		}

		if(!got_picture)
		{
			break;
		}

		sws_scale(img_convert_ctx, (const unsigned char * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

		int y_size = pCodecCtx->width * pCodecCtx->height;
		fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
		fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
		fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);

		printf("Flush Decoder : Succeed to decode 1 frame!\n");
	}

	sws_freeContext(img_convert_ctx);

	fclose(fp_yuv);

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

    return 0;
}
