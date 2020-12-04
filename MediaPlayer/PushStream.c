/*************************************************************************
	> File Name: send_stream.c
	> Author: 
	> Mail: 
	> Created Time: 五 11/20 14:29:09 2020
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>



int Close(AVFormatContext *ifmt_ctx, AVFormatContext *ofmt_ctx, AVOutputFormat *ofmt, int ret)
{
    avformat_close_input(&ifmt_ctx);

    if(ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
    {
        avio_close(ofmt_ctx->pb);
    }
    avformat_free_context(ofmt_ctx);
    if(ret < 0 && ret != AVERROR_EOF)
    {
        printf("Error occurred!\n");
        return -1;
    }
    return 0;
}



int main(int argc, char *argv[])
{
	AVOutputFormat *ofmt = NULL;
	AVFormatContext *ifmt_ctx = NULL;
	AVFormatContext *ofmt_ctx = NULL;
	AVPacket pkt;
	const char *in_filename = argv[1];
	const char *out_filename = NULL;
	int ret = 0;
	int i = 0;
	int videoindex = 0;
	int frame_index = 0;
	int64_t start_time = 0;

	out_filename = "rtmp://localhost:1935/live/room";


	av_register_all();

	avformat_network_init();

	if((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0)
	{
		printf("Could not open input file!");
		Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
	}

	if((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0)
	{
		printf("Failed to retrieve input stream information");
		Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
	}

	for(i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}

	av_dump_format(ifmt_ctx, 0, in_filename, 0);

	//给输出格式分配一个AVFormatContext机构体
	avformat_alloc_output_context2(&ofmt_ctx, NULL, "flv", out_filename);

	if(!ofmt_ctx)
	{
		printf("Could not create output context!\n");
		ret = AVERROR_UNKNOWN;
		Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
	}

	ofmt = ofmt_ctx->oformat;
	for(i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		AVStream *in_stream = ifmt_ctx->streams[i];
		//为作为输出的文件添加音视频流
		AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
		if(!out_stream)
		{
			printf("Failed allocating output stream!\n");
			ret = AVERROR_UNKNOWN;
			Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
		}

		ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
		if(ret < 0)
		{
			printf("Failed to copy context from input to output stream codec context!\n");
			Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
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
		//为从URL中获取到的资源创建和初始化一个AVIOContext结构体
		ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
		if(ret < 0)
		{
			printf("Could not open output URL '%s'\n", out_filename);
			Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
		}
	}

	//分配流的私有数据并将流的头部信息写入多媒体文件中
	ret = avformat_write_header(ofmt_ctx, NULL);
	if(ret < 0)
	{
		printf("error occurred when opening output URL!\n");
		Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
	}

	//获取当前时间（单位ms）
	start_time = av_gettime();
	while(1)
	{
		AVStream *in_stream = NULL;
		AVStream *out_stream = NULL;

		ret = av_read_frame(ifmt_ctx, &pkt);
		if(ret < 0)
		{
			break;
		}

		if(pkt.pts == AV_NOPTS_VALUE)
		{
			AVRational time_base1 = ifmt_ctx->streams[videoindex]->time_base;
			int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(ifmt_ctx->streams[videoindex]->r_frame_rate);
			pkt.pts = (double)(frame_index * calc_duration)/(double)(av_q2d(time_base1) * AV_TIME_BASE);
			pkt.dts = pkt.pts;
			pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
		}

		if(pkt.stream_index == videoindex)
		{
			AVRational time_base = ifmt_ctx->streams[videoindex]->time_base;
			AVRational time_base_q = {1, AV_TIME_BASE};
			int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
			int64_t now_time = av_gettime() - start_time;
			if(pts_time > now_time)
			{
				av_usleep(pts_time - now_time);
			}
		}

		in_stream = ifmt_ctx->streams[pkt.stream_index];
		out_stream = ofmt_ctx->streams[pkt.stream_index];

		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.duration = av_rescale_q_rnd(pkt.duration, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.pos = -1;

		if(pkt.stream_index == videoindex)
		{
			printf("Send %8d video frames to output URL!\n", frame_index);
			frame_index++;
		}

		ret = av_interleaved_write_frame(ofmt_ctx, &pkt);

		if(ret < 0)
		{
			printf("Error muxing packet!\n");
			break;
		}

		av_packet_unref(&pkt);
	}

	av_write_trailer(ofmt_ctx);

    return 0;
}
