/*************************************************************************
	> File Name: remuxer.c
	> 转换封装格式（本程序仅完成flv转mp4）
	> Author: 
	> Mail: 
	> Created Time: 二 12/22 13:52:54 2020
 ************************************************************************/

#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>


int main(int argc, char **argv)
{
	AVOutputFormat *ofmt = NULL;
	AVFormatContext *ifmt_ctx = NULL;
	AVFormatContext *ofmt_ctx = NULL;
	AVPacket pkt;
	const char *in_filename = "test.flv";
	const char *out_filename = "test.mp4";

	/*
	if(argc < 3)
	{
		printf("Usage : ./%s src dest\n", *argv);
		return -1;
	}
	*/

	av_register_all();
	
	if(avformat_open_input(&ifmt_ctx, in_filename, NULL, NULL) < 0)
	{
		printf("open input stream information error!\n");
		return -1;
	}

	if(avformat_find_stream_info(ifmt_ctx, NULL) < 0)
	{
		printf("find stream information error!\n");
		return -1;
	}

	av_dump_format(ifmt_ctx, 0, in_filename, 0);

	avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
	if(!ofmt_ctx)
	{
		printf("alloc output context error!\n");
		return -1;
	}

	ofmt = ofmt_ctx->oformat;

	for(int i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		AVStream *in_stream = ifmt_ctx->streams[i];
		AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
		if(!out_stream)
		{
			printf("alloc output stream error!\n");
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

	av_dump_format(ofmt_ctx, 0, out_filename, 1);

	if(!(ofmt->flags & AVFMT_NOFILE))
	{
		if(avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE) < 0)
		{
			printf("open output file error!\n");
			return -1;
		}
	}

	if(avformat_write_header(ofmt_ctx, NULL) < 0)
	{
		printf("write header error!\n");
		return -1;
	}

	int frame_index = 0;
	while(1)
	{
		AVStream *in_stream = NULL;
		AVStream *out_stream = NULL;

		if(av_read_frame(ifmt_ctx, &pkt) < 0)
		{
			printf("read frame error!\n");
			break;
		}

		in_stream = ifmt_ctx->streams[pkt.stream_index];
		out_stream = ofmt_ctx->streams[pkt.stream_index];

		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;

		if(av_interleaved_write_frame(ofmt_ctx, &pkt) < 0)
		{
			printf("muxing packet error!\n");
			break;
		}

		printf("Write %8d frames to output file!\n", frame_index);
		av_free_packet(&pkt);
		frame_index++;
	}

	av_write_trailer(ofmt_ctx);

	avio_close(ofmt_ctx->pb);
	avformat_free_context(ofmt_ctx);

    return 0;
}
