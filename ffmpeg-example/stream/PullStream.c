/*
	拉一个rtmp流
*/

#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
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


int main(int argc, char** argv)
{
    AVOutputFormat *ofmt = NULL;
    AVFormatContext *ifmt_ctx = NULL;
    AVFormatContext *ofmt_ctx = NULL;
    AVPacket pkt;
    const char *in_filename = "rtmp://localhost:1935/live/room";
    const char *out_filename = "receive.flv";
    int videoindex = -1;
    int frame_index = 0;
	int ret = 0;


    av_register_all();

    avformat_network_init();


    if(avformat_open_input(&ifmt_ctx, in_filename, 0, 0) < 0)
    {
        printf("could not open input file.");
        Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
    }

    if(avformat_find_stream_info(ifmt_ctx, 0) < 0)
    {
        printf("Failed to retrieve input stream information");
        Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
    }

    for(int i = 0; i < ifmt_ctx->nb_streams; i++)
    {
        if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoindex = i;
            break;
        }
    }

	if(-1 == videoindex)
	{
		printf("Could not find stream!\n");
		Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
	}

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

	//为输出格式分配一个AVFormatContext结构体
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
    if(!ofmt_ctx)
    {
        printf("Could not create output context!\n");
        ret = AVERROR_UNKNOWN;
        Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
    }

    ofmt = ofmt_ctx->oformat;

    for(int i = 0; i < ifmt_ctx->nb_streams; i++)
    {
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
        if(!out_stream)
        {
            printf("Failed allocating output stream!\n");
            ret = AVERROR_UNKNOWN;
			Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
        }

		if(avcodec_copy_context(out_stream->codec, in_stream->codec) < 0)
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
		if(avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE) < 0)
		{
			printf("Error occured when opening output URL!\n");
			Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
		}
	}

	if(avformat_write_header(ofmt_ctx, NULL) < 0)
	{
		printf("Error occurred when opening output URL!\n");
		Close(ifmt_ctx, ofmt_ctx, ofmt, ret);
	}


	while(1)
	{
		AVStream *in_stream = NULL;
		AVStream *out_stream = NULL;
		if(av_read_frame(ifmt_ctx, &pkt) < 0)
		{
			break;
		}

		in_stream = ifmt_ctx->streams[pkt.stream_index];
		out_stream = ofmt_ctx->streams[pkt.stream_index];

		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;

		if(videoindex == pkt.stream_index)
		{
			printf("Receive %8d video frames from input URL!\n", frame_index);
			frame_index++;
		}

		if(av_interleaved_write_frame(ofmt_ctx, &pkt) < 0)
		{
			printf("Error muxing packet!\n");
			break;
		}

		av_free_packet(&pkt);
	}


	av_write_trailer(ofmt_ctx);
    return 0;
}
