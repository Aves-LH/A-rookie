#include "StreamOperate.h"



int stream_component_open(VideoState *is, int stream_index) 
{
    
    AVFormatContext *pFormatCtx = is->pFormatCtx;
    AVCodecContext *codecCtx = NULL;
    AVCodec *codec = NULL;
    SDL_AudioSpec wanted_spec, spec;
    
    if(stream_index < 0 || stream_index >= pFormatCtx->nb_streams) 
    { 
        return -1;
    }
    
    codecCtx = avcodec_alloc_context3(NULL);
    
    
    int ret = avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[stream_index]->codecpar);
    if (ret < 0)
    {
        return -1;
    }
        
    
    codec = avcodec_find_decoder(codecCtx->codec_id); 
    if(!codec) 
    {
        fprintf(stderr, "Unsupported codec!\n");
        return -1;
    }
    
    if(avcodec_open2(codecCtx, codec, NULL) < 0) 
    { 
        fprintf(stderr, "Unsupported codec!\n");
        return -1;
    }
    
    switch(codecCtx->codec_type) 
    {
        case AVMEDIA_TYPE_AUDIO:
            {
            wanted_spec.freq = codecCtx->sample_rate; 
            wanted_spec.format = AUDIO_S16SYS; 
            wanted_spec.channels = 2;
            wanted_spec.silence = 0;
            wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE; 
            wanted_spec.callback = audio_callback; 
            wanted_spec.userdata = is;
            
            fprintf(stderr, "wanted spec: channels:%d, sample_fmt:%d, sample_rate:%d \n",
                    2, AUDIO_S16SYS, codecCtx->sample_rate);
            
            if(SDL_OpenAudio(&wanted_spec, &spec) < 0) 
            {  
                fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
                return -1;
            }
            is->audio_hw_buf_size = spec.size;
            
            is->audioStream = stream_index; 
            is->audio_st = pFormatCtx->streams[stream_index];
            is->audio_ctx = codecCtx; 
            is->audio_buf_size = 0;
            is->audio_buf_index = 0;
            memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
            QueueInit(&is->audioq); 
            
            uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
            
            int out_nb_samples= is->audio_ctx->frame_size;
            
            int out_sample_rate=is->audio_ctx->sample_rate;
            int out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
            
            
            int64_t in_channel_layout=av_get_default_channel_layout(is->audio_ctx->channels);
            
            struct SwrContext *audio_convert_ctx;  
            audio_convert_ctx = swr_alloc();
            swr_alloc_set_opts(audio_convert_ctx,
                               out_channel_layout,
                               AV_SAMPLE_FMT_S16,
                               out_sample_rate,
                               in_channel_layout,
                               is->audio_ctx->sample_fmt, 
                               is->audio_ctx->sample_rate,
                               0,
                               NULL);
            fprintf(stderr, "swr opts: out_channel_layout:%lld, out_sample_fmt:%d, out_sample_rate:%d, in_channel_layout:%lld, in_sample_fmt:%d, in_sample_rate:%d",
                    out_channel_layout, AV_SAMPLE_FMT_S16, out_sample_rate, in_channel_layout, is->audio_ctx->sample_fmt, is->audio_ctx->sample_rate);
            swr_init(audio_convert_ctx);
            
            is->audio_swr_ctx = audio_convert_ctx;
            
            SDL_PauseAudio(0);
            }
            break;
        case AVMEDIA_TYPE_VIDEO:
            {
                is->videoStream = stream_index;
                is->video_st = pFormatCtx->streams[stream_index];
                is->video_ctx = codecCtx;
                
                is->frame_timer = (double)av_gettime() / 1000000.0; 
    #if DEBUG1
                printf(" \n av_gettime() = %d -- \n",av_gettime());
                printf(" \n is->frame_timer = %lf -- \n",is->frame_timer);
    #endif
                is->frame_last_delay = 40e-3;
                is->video_current_pts_time = av_gettime();
                
                QueueInit(&is->videoq);
                
                printf("\n is->video_ctx->width = %d --\n", is->video_ctx->width);
                
                
                is->video_sws_ctx = sws_getContext(is->video_ctx->width, is->video_ctx->height,
                                                is->video_ctx->pix_fmt,  is->video_ctx->width,
                                                is->video_ctx->height , AV_PIX_FMT_YUV420P,
                                                SWS_BILINEAR, NULL, NULL, NULL
                                                );
                
                is->video_tid = SDL_CreateThread(decode_video_thread, "decode_video_thread", is); 
            }
            break;
        default:
            break;
    }
    return 0;
}

//解复用线程
int demux_thread(void *arg) 
{
    
    int err_code;
    char errors[1024] = {0,};
    
    VideoState *is = (VideoState *)arg;
    AVFormatContext *pFormatCtx = NULL;
    AVPacket pkt1, *packet = &pkt1;
    
    int video_index = -1;
    int audio_index = -1;
    int i;
    
    is->videoStream=-1;
    is->audioStream=-1;
    
    global_video_state = is;
    
    if ((err_code=avformat_open_input(&pFormatCtx, is->filename, NULL, NULL)) < 0) 
    {
        av_strerror(err_code, errors, 1024);
        fprintf(stderr, "Could not open source file %s, %d(%s)\n", is->filename, err_code, errors);
        return -1;
    }
    
    is->pFormatCtx = pFormatCtx;
    
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
    {
        return -1;
    }    
    
    av_dump_format(pFormatCtx, 0, is->filename, 0);
    
    
    for(i=0; i<pFormatCtx->nb_streams; i++) 
    {
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO && video_index < 0) 
        {
            video_index=i;
        }
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO && audio_index < 0) 
        {
            audio_index=i;
        }
    }
    if(audio_index >= 0) 
    {
        stream_component_open(is, audio_index);
    }
    if(video_index >= 0) 
    {
        stream_component_open(is, video_index);
    }
    
    if(is->videoStream < 0 || is->audioStream < 0) 
    { 
        fprintf(stderr, "%s: could not open codecs\n", is->filename);
        goto fail;
    }
    
    screen_width = is->video_ctx->width ;
    screen_height = is->video_ctx->height; 
    
    
    while(1) 
    { 
        if(is->quit) 
        {
            break;
        }
        if(is->audioq.size > MAX_AUDIOQ_SIZE || is->videoq.size > MAX_VIDEOQ_SIZE) 
        {
            SDL_Delay(10);
            continue;
        }
        if(av_read_frame(is->pFormatCtx, packet) < 0) 
        { 
            if(is->pFormatCtx->pb->error == 0) 
            {
                SDL_Delay(100); 
                continue;
            } 
            else 
            {
                break;
            }
        }
        if(packet->stream_index == is->videoStream) 
        { 
            QueuePush(&is->videoq, packet);
        } 
        else if(packet->stream_index == is->audioStream) 
        { 
            QueuePush(&is->audioq, packet);
        } 
        else 
        {
            av_free_packet(packet);
        }
    }
    while(!is->quit) 
    {
        SDL_Delay(100);
    }
    
fail:
    if(1)
    {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        event.user.data1 = is;
        
        SDL_PushEvent(&event);
        printf("PUSH_QUIT_EVENT\n");
    }
    return 0;
}


