/*************************************************************************
	> File Name: MediaPlayer.c
	> Author: 
	> Mail: 
	> Created Time: 三 11/25 14:20:50 2020
 ************************************************************************/

#include "Video.h"
#include "Audio.h"
#include "StreamOperate.h"

SDL_mutex    *text_mutex = NULL;
SDL_Window   *win = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture  *texture = NULL;

static uint32_t sdl_refresh_timer_cb(uint32_t interval, void *opaque) 
{
	SDL_Event event;
    event.type = FF_REFRESH_EVENT;
    event.user.data1 = opaque;
    SDL_PushEvent(&event);
    return 0; 
}

static void schedule_refresh(VideoState *is, int delay)
{
    SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}

void video_refresh_timer(void *userdata)
{
	VideoState *is = (VideoState *)userdata;
    VideoPicture *vp;
    double actual_delay, delay, sync_threshold, ref_clock, diff;
    
    if(is->video_st) 
    {
        if(is->pictq_size == 0) 
        {
            schedule_refresh(is, 1);
        } 
        else 
        {
            vp = &is->pictq[is->pictq_rindex];
            
            is->video_current_pts = vp->pts;
            is->video_current_pts_time = av_gettime();
           
            delay = vp->pts - is->frame_last_pts; 

            if(delay <= 0 || delay >= 1.0) 
            { 
                delay = is->frame_last_delay;
            }
            is->frame_last_delay = delay;
            is->frame_last_pts = vp->pts;
            
            if(is->av_sync_type != AV_SYNC_VIDEO_MASTER) 
            {
                ref_clock = get_audio_clock(is);
                diff = vp->pts - ref_clock; 

                sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
                

                if(fabs(diff) < AV_NOSYNC_THRESHOLD) 
                { 
                    if(diff <= -sync_threshold) 
                    { 
                        delay = 0;
                    } 
                    else if(diff >= sync_threshold) 
                    { 
                        delay = 2 * delay;
                    }
                }
            }
            //系统时间加上delay
            is->frame_timer += delay;
            actual_delay = is->frame_timer - (av_gettime() / 1000000.0);
            if(actual_delay < 0.010) 
            {
                actual_delay = 0.010; 
            }

            schedule_refresh(is, (int)(actual_delay * 1000 + 0.5)); 
            
            video_display(is); 
            
            if(++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE) 
            {
                is->pictq_rindex = 0;
            }
            SDL_LockMutex(is->pictq_mutex);
            is->pictq_size--;
            SDL_CondSignal(is->pictq_cond);
            SDL_UnlockMutex(is->pictq_mutex);
        }
    } 
    else 
    {
        schedule_refresh(is, 100);
    }

}

VideoState *global_video_state = NULL;



int main(int argc, char **argv)
{
	const char * path = "rtmp://localhost:1935/live/room";
    
    SDL_Event       event;
    
    VideoState      *is;
    
    is = (VideoState *)av_mallocz(sizeof(VideoState));
    
    av_register_all();
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
    {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }
    
    win = SDL_CreateWindow("Media Player",
                           100,
                           100,
                           0,0,
                           SDL_WINDOW_RESIZABLE);
    if(!win) 
    {
        fprintf(stderr, "\nSDL: could not set video mode:%s - exiting\n", SDL_GetError());
        exit(1);
    }
    
    renderer = SDL_CreateRenderer(win, -1, 0);
    
    text_mutex = SDL_CreateMutex();
    
    av_strlcpy(is->filename, path, sizeof(is->filename));
    
    is->pictq_mutex = SDL_CreateMutex(); 
    is->pictq_cond = SDL_CreateCond();
    
    schedule_refresh(is, 40);
    
    is->av_sync_type = DEFAULT_AV_SYNC_TYPE;
    is->parse_tid = SDL_CreateThread(demux_thread,"demux_thread", is); 
    if(!is->parse_tid) 
    {
        av_free(is);
        return -1;
    }

    while(1)
    {
        SDL_WaitEvent(&event);
        switch(event.type) 
        {
            case FF_QUIT_EVENT:
            case SDL_QUIT:
                is->quit = 1;
                
                
                printf("SDL_Quit --Receive---\n");
                SDL_Quit();
               
                return 0;
                break;
            case FF_REFRESH_EVENT:
                video_refresh_timer(event.user.data1);
                break;
            default:
                break;
        }
    }
    
    return 0;
}
