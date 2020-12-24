/*
	拉视频流并播放
*/



#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>


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


int main(int argc, char** argv)
{
	AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *pFrame = NULL;
    AVFrame *pFrameYUV = NULL;
    AVPacket *packet = NULL;
    AVFormatContext *pFormatCtx = NULL;
	unsigned char *out_buf = NULL;
    struct SwsContext *img_convert_ctx = NULL;  //转换器参数结构体
    
    int y_size = 0;
    int got_pic = 0;       //解码操作是否成功标志位
    int frame_index = 0;
    const char *in_filename = "rtmp://localhost:1935/live/room";

    av_register_all();

    //初始化网络库
    avformat_network_init();

    //打开视频文件，读取文件头内容，读取文件容器的封装信息和码流参数并存储在pFormatCtx中
    if(avformat_open_input(&pFormatCtx, in_filename, 0, 0) < 0)
    {
        printf("could not open input file.");
        avformat_close_input(&pFormatCtx);
        return -1;
    }

    //读取文件中保存的码流信息，并写入pFormatCtx->stream
    if(avformat_find_stream_info(pFormatCtx, 0) < 0)
    {
        printf("Failed to retrieve input stream information");
        avformat_close_input(&pFormatCtx);
        return -1;
    }

	int videoindex = -1;
    for(int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        //遍历文件中所有的流媒体类型，若编码类型与AVMEDIA_TYPE_VIDEO相等，则将值赋给videoindex
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoindex = i;
            break;
        }
    }
	if(-1 == videoindex)
	{
		printf("Could find stream!\n");
		avformat_close_input(&pFormatCtx);
		return -1;
	}

    //根据流类型标号（videoindex）从pFormatCtx中获取编解码器上下文并赋值给pCodecCtx
    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
    //根据对应的解码器上下文获取解码器信息
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(NULL == pCodec)
    {
        printf("Codec not found!\n");
        return -1;
    }

    //打开解码器
	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec!\n");
		return -1;
	}

	pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();

    //根据像素格式及图像尺寸计算所需内存大小并分配内存控件
    out_buf = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    //为AVFrame对象安装图像缓存，将out_buffer缓存挂到pFrameYUV->data指针上
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buf, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    printf("=====================File Stream=======================\n");
    av_dump_format(pFormatCtx, 0, in_filename, 0);
    printf("=====================================\n");

    //设置图像像素转换格式为YUV420P
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);


	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        printf("Could not init sdl : %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("MediaPlayer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pCodecCtx->width, pCodecCtx->height, SDL_WINDOW_OPENGL);
    if(!window)
    {
        printf("Could not set video mode : %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_TARGET, pCodecCtx->width, pCodecCtx->height);
    
    SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video, NULL, NULL);
	SDL_Event event;

    
	while(1)
	{
		ret = av_read_frame(pFormatCtx, packet);
		if(ret < 0)
		{
			break;
		}

        if(packet->stream_index == videoindex)
        {
            //解码一帧完整的视频数据，并将got_pic标志位设置为true
            //若一个packet无法解码出一帧完整数据，则会从缓存队列中读取多个packet直至解码一帧完整数据
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_pic, packet);
            if(ret < 0)
            {
                printf("Decode Error!\n");
                return -1;
            }

            if(got_pic) //校验是否解码出一帧完整图像
            {
                //将解码后的图片格式转换为YUV420P格式
                sws_scale(img_convert_ctx, (const unsigned char * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
				
                //等待事件发生
                if(!SDL_WaitEventTimeout(&event, 1))
                {
                    break;
                }
                //若发生的事件为刷新事件
                if(event.type == REFRESH_EVENT)
                {
                    //刷新纹理，渲染画面
				    SDL_UpdateYUVTexture(texture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0], pFrameYUV->data[1], pFrameYUV->linesize[1], pFrameYUV->data[2], pFrameYUV->linesize[2]);
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(40);
                }
                else if(event.type == SDL_WINDOWEVENT)
                {
                    SDL_GetWindowSize(window, NULL, NULL);
                }
                else if(event.type == SDL_QUIT)
                {
                    break;
                }
            }
        }

		av_free_packet(packet);
	}

    sws_freeContext(img_convert_ctx);
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
