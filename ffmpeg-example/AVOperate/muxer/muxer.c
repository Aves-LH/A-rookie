/*************************************************************************
	> File Name: muxer.c
	> 将音视频文件封装
	> Author: 
	> Mail: 
	> Created Time: 二 12/22 15:05:55 2020
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
	AVFormatContext *ifmt_ctx_v = NULL;
	AVFormatContext *ifmt_ctx_a = NULL;
	AVFormatContext *ofmt_ctx = NULL;
	AVPacket pkt;
	int64_t cur_pts_v = 0;
	int64_t cur_pts_a = 0;

	const char *in_filename_v = "test.h264";
	const char *in_filename_a = "test.mp3";
	const char *out_filename = "test.mp4";

	av_register_all();

	if(avformat_open_input(&ifmt_ctx_v, in_filename_v, NULL, NULL) < 0)
	{
		printf("open video input stream error!\n");
		return -1;
	}

	if(avformat_find_stream_info(ifmt_ctx_v, NULL) < 0)
	{
		printf("find video stream information error!\n");
		return -1;
	}

	if(avformat_open_input(&ifmt_ctx_a, in_filename_a, NULL, NULL) < 0)
	{
		printf("open audio input stream error!\n");
		return -1;
	}

	if(avformat_find_stream_info(ifmt_ctx_a, NULL) < 0)
	{
		printf("find audio stream information error!\n");
		return -1;
	}

	printf("=============Input Information===============\n");
	av_dump_format(ifmt_ctx_v, 0, in_filename_v, 0);
	av_dump_format(ifmt_ctx_a, 0, in_filename_a, 0);
	printf("=============================================\n");

	avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
	if(!ofmt_ctx)
	{
		printf("alloc output context error!\n");
		return -1;
	}
	ofmt = ofmt_ctx->oformat;

	int videoindex_v = 0;
	int videoindex_out = 0;
	for(int i = 0; i < ifmt_ctx_v->nb_streams; i++)
	{
		if(ifmt_ctx_v->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			AVStream *in_stream = ifmt_ctx_v->streams[i];
			AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
			videoindex_v = i;
			if(!out_stream)
			{
				printf("alloc output stream!\n");
				return -1;
			}

			videoindex_out = out_stream->index;
 
			if(avcodec_copy_context(out_stream->codec, in_stream->codec) < 0)
			{
				printf("copy video context error!\n");
				return -1;
			}

			out_stream->codec->codec_tag = 0;
			if(ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			{
				out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
			}
			break;
		}
	}

	int audioindex_a = 0;
	int audioindex_out = 0;
	for(int i = 0; i < ifmt_ctx_a->nb_streams; i++)
	{
		if(ifmt_ctx_a->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			AVStream *in_stream = ifmt_ctx_a->streams[i];
			AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);

			audioindex_a = i;

			if(!out_stream)
			{
				printf("alloc output stream!\n");
				return -1;
			}

			audioindex_out = out_stream->index;

			if(avcodec_copy_context(out_stream->codec, in_stream->codec) < 0)
			{
				printf("copy audio context error!\n");
				return -1;
			}

			out_stream->codec->codec_tag = 0;

			if(ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			{
				out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
			}

			break;
		}
	}

	printf("=================Output Information===================\n");
	av_dump_format(ofmt_ctx, 0, out_filename, 1);
	printf("======================================================\n");

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
		AVFormatContext *ifmt_ctx = NULL;
		int stream_index = 0;
		AVStream *in_stream = NULL;
		AVStream *out_stream = NULL;

		if(av_compare_ts(cur_pts_v, ifmt_ctx_v->streams[videoindex_v]->time_base, cur_pts_a, ifmt_ctx_a->streams[audioindex_a]->time_base) <= 0)
		{
			ifmt_ctx = ifmt_ctx_v;
			stream_index = videoindex_out;

			if(av_read_frame(ifmt_ctx, &pkt) >= 0)
			{
				do
				{
					in_stream = ifmt_ctx->streams[pkt.stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if(pkt.stream_index == videoindex_v)
					{
						if(pkt.pts == AV_NOPTS_VALUE)
						{
							AVRational time_base1 = in_stream->time_base;
							int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(in_stream->r_frame_rate);
							pkt.pts = (double)(frame_index * calc_duration) / (double)(av_q2d(time_base1) * AV_TIME_BASE);
							pkt.dts = pkt.pts;
							pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
							frame_index++;
						}

						cur_pts_v = pkt.pts;
						break;
					}
				}while(av_read_frame(ifmt_ctx, &pkt) >= 0);
			}
			else
			{
				break;
			}
		}
		else
		{
			ifmt_ctx = ifmt_ctx_a;
			stream_index = audioindex_out;
			if(av_read_frame(ifmt_ctx, &pkt) >= 0)
			{
				do
				{
					in_stream = ifmt_ctx->streams[pkt.stream_index];
					out_stream = ofmt_ctx->streams[stream_index];
	
					if(pkt.stream_index == audioindex_a)
					{
						if(pkt.pts == AV_NOPTS_VALUE)
						{
							AVRational time_base1 = in_stream->time_base;
							int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(in_stream->r_frame_rate);
							pkt.pts = (double)(frame_index * calc_duration) / (double)(av_q2d(time_base1) * AV_TIME_BASE);
							pkt.dts = pkt.pts;
							pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
							frame_index++;
						}
						cur_pts_a = pkt.pts;
						break;
					}
				}while(av_read_frame(ifmt_ctx, &pkt) >= 0);
			}
			else
			{
				break;
			}
		}

		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;
		pkt.stream_index = stream_index;

		printf("Write 1 Packet. size : %5d\tpts : %lld\n", pkt.size, pkt.pts);
		if(av_interleaved_write_frame(ofmt_ctx, &pkt) < 0)
		{
			break;
		}

		av_free_packet(&pkt);
	}

	av_write_trailer(ofmt_ctx);

	avformat_close_input(&ifmt_ctx_v);
	avformat_close_input(&ifmt_ctx_a);
	avformat_free_context(ofmt_ctx);

    return 0;
}
