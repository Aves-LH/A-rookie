/*************************************************************************
	> File Name: demuxer.c
	> 将视频文件解封装（ts文件解封装为h264格式的视频文件和aac格式的音频文件）
	> Author: 
	> Mail: 
	> Created Time: 二 12/22 15:08:14 2020
 ************************************************************************/

#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>


int main(int argc, char **argv)
{
	AVOutputFormat *ofmt_a = NULL;
	AVOutputFormat *ofmt_v = NULL;
	AVFormatContext *ifmt_ctx = NULL;
	AVFormatContext *ofmt_ctx_a = NULL;
	AVFormatContext *ofmt_ctx_v = NULL;
	AVPacket pkt;

	const char *in_filename = "test.ts";
	const char *out_filename_v = "test.h264";
	const char *out_filename_a = "test.aac";

	av_register_all();
	
	if(avformat_open_input(&ifmt_ctx, in_filename, NULL, NULL) < 0)
	{
		printf("open input stream error!\n");
		return -1;
	}

	if(avformat_find_stream_info(ifmt_ctx, 0) < 0)
	{
		printf("find stream information error!\n");
		return -1;
	}

	avformat_alloc_output_context2(&ofmt_ctx_v, NULL, NULL, out_filename_v);
	if(!ofmt_ctx_v)
	{
		printf("alloc video output context error!\n");
		return -1;
	}
	ofmt_v = ofmt_ctx_v->oformat;

	avformat_alloc_output_context2(&ofmt_ctx_a, NULL, NULL, out_filename_a);
	if(!ofmt_ctx_a)
	{
		printf("alloc audio output context error!\n");
		return -1;
	}
	ofmt_a = ofmt_ctx_a->oformat;

	int videoindex = -1;
	int audioindex = -1;
	for(int i = 0; i <ifmt_ctx->nb_streams; i++)
	{
		AVFormatContext *ofmt_ctx = NULL;
		AVStream *in_stream = ifmt_ctx->streams[i];
		AVStream *out_stream = NULL;

		if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			out_stream = avformat_new_stream(ofmt_ctx_v, in_stream->codec->codec);
			ofmt_ctx = ofmt_ctx_v;
		}
		else if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioindex = i;
			out_stream = avformat_new_stream(ofmt_ctx_a, in_stream->codec->codec);
			ofmt_ctx = ofmt_ctx_a;
		}
		else
		{
			break;
		}

		if(!out_stream)
		{
			printf("alloc output stream errro!\n");
			return -1;
		}

		if(avcodec_copy_context(out_stream->codec, in_stream->codec) < 0)
		{
			printf("copy context error!\n");
			return -1;
		}

		out_stream->codec->codec_tag = 0;
		if(ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
		{
			out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		}
	}
	
	printf("\n==============Input Video===============\n");
	av_dump_format(ifmt_ctx, 0, in_filename, 0);
	printf("\n==============Onput Video===============\n");
	av_dump_format(ofmt_ctx_v, 0, out_filename_v, 1);
	printf("\n==============Onput Audio===============\n");
	av_dump_format(ofmt_ctx_a, 0, out_filename_a, 1);
	printf("\n========================================\n");

	if(!(ofmt_v->flags & AVFMT_NOFILE))
	{
		if(avio_open(&ofmt_ctx_v->pb, out_filename_v, AVIO_FLAG_WRITE) < 0)
		{
			printf("open output video file error!\n");
			return -1;
		}
	}

	if(!(ofmt_a->flags & AVFMT_NOFILE))
	{
		if(avio_open(&ofmt_ctx_a->pb, out_filename_a, AVIO_FLAG_WRITE) < 0)
		{
			printf("open output audio file error!\n");
			return -1;
		}
	}
	
	if(avformat_write_header(ofmt_ctx_v, NULL) < 0)
	{
		printf("write header to video file error!\n");
		return -1;
	}

	if(avformat_write_header(ofmt_ctx_a, NULL) < 0)
	{
		printf("write header to audio file erro!\n");
		return -1;
	}

	int frame_index = 0;
	while(1)
	{
		AVFormatContext *ofmt_ctx = NULL;
		AVStream *in_stream = NULL;
		AVStream *out_stream = NULL;

		if(av_read_frame(ifmt_ctx, &pkt) < 0)
		{
			printf("read frame error!\n");
			break;
		}
		in_stream = ifmt_ctx->streams[pkt.stream_index];

		if(pkt.stream_index == videoindex)
		{
			out_stream = ofmt_ctx_v->streams[0];
			ofmt_ctx = ofmt_ctx_v;
			printf("Write Video Packet. size : %d\tpts : %lld\n", pkt.size, pkt.pts);
		}
		else if(pkt.stream_index == audioindex)
		{
			out_stream = ofmt_ctx_a->streams[0];
			ofmt_ctx = ofmt_ctx_a;
			printf("Write Audio Packet. size : %d\tpts:%lld\n", pkt.size, pkt.pts);
		}
		else
		{
			continue;
		}

		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;
		pkt.stream_index = 0;

		if(av_interleaved_write_frame(ofmt_ctx, &pkt) < 0)
		{
			printf("muxing packet error!\n");
			break;
		}
		av_free_packet(&pkt);
		frame_index++;
	}

	av_write_trailer(ofmt_ctx_a);
	av_write_trailer(ofmt_ctx_v);

	avformat_close_input(&ifmt_ctx);
	if(ofmt_ctx_a && !(ofmt_a->flags & AVFMT_NOFILE))
	{
		avio_close(ofmt_ctx_a->pb);
	}
	if(ofmt_ctx_v && !(ofmt_v->flags & AVFMT_NOFILE))
	{
		avio_close(ofmt_ctx_v->pb);
	}
	avformat_free_context(ofmt_ctx_a);
	avformat_free_context(ofmt_ctx_v);

    return 0;
}
