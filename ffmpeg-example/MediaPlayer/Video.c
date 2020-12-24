#include "Video.h"



void video_display(VideoState *is)
{
	SDL_Rect rect;
    VideoPicture *vp;
    float aspect_ratio;
    int w, h, x, y;
    int i;
    
    if(screen_width && resize)
    {
        SDL_SetWindowSize(win, PLAYER_WIDTH, PLAYER_HEIGHT); 
        
        SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, screen_top);
        SDL_ShowWindow(win);
        
        uint32_t pixformat= SDL_PIXELFORMAT_IYUV;
        
        texture = SDL_CreateTexture(renderer,
                                    pixformat,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    screen_width ,
                                    screen_height);
        resize = 0;
    }
    
    vp = &is->pictq[is->pictq_rindex];
    if(vp->bmp) 
    {
        SDL_UpdateYUVTexture( texture, NULL,
                             vp->bmp->data[0], vp->bmp->linesize[0],
                             vp->bmp->data[1], vp->bmp->linesize[1],
                             vp->bmp->data[2], vp->bmp->linesize[2]);
        
        rect.x = 0;
        rect.y = 0;
        rect.w = PLAYER_WIDTH;
        rect.h = PLAYER_HEIGHT;
        SDL_LockMutex(text_mutex);//刷一次屏
        SDL_RenderCopy( renderer, texture, NULL, &rect);
        SDL_RenderPresent( renderer );
        SDL_UnlockMutex(text_mutex);
        
    }

}



void alloc_picture(void *userdata)
{
	int ret;
    
    VideoState *is = (VideoState *)userdata;
    VideoPicture *vp;
    
    vp = &is->pictq[is->pictq_windex];
    if(vp->bmp) 
    {
        avpicture_free(vp->bmp);
        free(vp->bmp);
        
        vp->bmp = NULL;
    }
    
    SDL_LockMutex(text_mutex);
    
    vp->bmp = (AVPicture*)malloc(sizeof(AVPicture));
    ret = avpicture_alloc(vp->bmp, AV_PIX_FMT_YUV420P, is->video_ctx->width, is->video_ctx->height);
    if (ret < 0) 
    {
        fprintf(stderr, "Could not allocate temporary picture: %s\n", av_err2str(ret));
    }
    
    SDL_UnlockMutex(text_mutex);
    
    vp->width = is->video_ctx->width ;
    vp->height = is->video_ctx->height ;
    vp->allocated = 1;
    

}

int queue_picture(VideoState *is, AVFrame *pFrame, double pts)
{
	VideoPicture *vp;
    
    SDL_LockMutex(is->pictq_mutex);
    while(is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE && !is->quit) 
    {
        SDL_CondWait(is->pictq_cond, is->pictq_mutex);
    }
    SDL_UnlockMutex(is->pictq_mutex);
    
    if(is->quit) 
    {
        return -1;
    }
        
    
    vp = &is->pictq[is->pictq_windex];
    
    if(!vp->bmp || vp->width != is->video_ctx->width || vp->height != is->video_ctx->height) 
    {
        
        vp->allocated = 0;
        alloc_picture(is);
        if(is->quit) 
        {
            return -1;
        }
    }
    
    if(vp->bmp) 
    {
        
        vp->pts = pts;
        
        sws_scale(is->video_sws_ctx, (uint8_t const * const *)pFrame->data,
                  pFrame->linesize, 0, is->video_ctx->height ,
                  vp->bmp->data, vp->bmp->linesize);
        
        if(++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) 
        {
            is->pictq_windex = 0;
        }
        
        SDL_LockMutex(is->pictq_mutex);
        is->pictq_size++;
        SDL_UnlockMutex(is->pictq_mutex);
    }
    return 0;

}

double synchronize_video(VideoState *is, AVFrame *src_frame, double pts)
{
	double frame_delay;
    
    if(pts != 0) 
    {
        is->video_clock = pts; 
    } 
    else 
    {
        pts = is->video_clock; 
    }
    frame_delay = av_q2d(is->video_ctx->time_base); 
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5); 

}

int decode_video_thread(void *arg)
{
	VideoState *is = (VideoState *)arg;
    AVPacket pkt1, *packet = &pkt1;
    int frameFinished;
    AVFrame *pFrame;
    double pts;
    
    pFrame = av_frame_alloc();
    
    while(1)
    {
        if(QueuePop(&is->videoq, packet, 1) < 0) 
        { 
            break;
        }
        pts = 0;
        
        avcodec_decode_video2(is->video_ctx, pFrame, &frameFinished, packet); 
        
        if((pts = av_frame_get_best_effort_timestamp(pFrame)) != AV_NOPTS_VALUE) 
        {
        } 
        else 
        {
            pts = 0;
        }
        pts *= av_q2d(is->video_st->time_base); 

        
        if(frameFinished) 
        {
            pts = synchronize_video(is, pFrame, pts);
            if(queue_picture(is, pFrame, pts) < 0) 
            { 
                break;
            }
        }
        av_free_packet(packet);
    }
    av_frame_free(&pFrame);
    return 0;

}


