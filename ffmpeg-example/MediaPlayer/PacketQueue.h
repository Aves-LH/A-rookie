#ifndef __PACKETQUEUE_H__
#define __PACKETQUEUE_H__

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <SDL2/SDL.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/time.h>
#include <libavutil/avstring.h>



#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000 //channels(2) * data_size(2) * sample_rate(48000)

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

#define AV_SYNC_THRESHOLD 0.01
#define AV_NOSYNC_THRESHOLD 10.0

#define SAMPLE_CORRECTION_PERCENT_MAX 10
#define AUDIO_DIFF_AVG_NB 20

#define FF_REFRESH_EVENT (SDL_USEREVENT)
#define FF_QUIT_EVENT (SDL_USEREVENT + 1)

#define VIDEO_PICTURE_QUEUE_SIZE 1

#define DEFAULT_AV_SYNC_TYPE AV_SYNC_AUDIO_MASTER 

#define PLAYER_WIDTH 1080 

#define PLAYER_HEIGHT 608 

#define  DEBUG1 0

extern int screen_left;
extern int screen_top;
extern int screen_width;
extern int screen_height;
extern int resize;
extern SDL_mutex    *text_mutex;
extern SDL_Window   *win;
extern SDL_Renderer *renderer;
extern SDL_Texture  *texture;

typedef struct PacketQueue 
{
    AVPacketList *first_pkt, *last_pkt; //队列中第一个pkt和最后一盒pkt
    int nb_packets;//队列中有多少个packet
    int size;  //总字节大小
    SDL_mutex *mutex; //互斥
    SDL_cond *cond; //查看源码、有示例
} PacketQueue;

typedef struct VideoPicture 
{
    AVPicture *bmp;
    int width, height; /* source height & width */
    int allocated;
    double pts;
} VideoPicture;

typedef struct VideoState 
{
    char            filename[1024];
    AVFormatContext *pFormatCtx;
    int             videoStream, audioStream;
  
    int             av_sync_type;
    
    double          audio_diff_cum; 
    double          audio_diff_avg_coef; 
    double          audio_diff_threshold;
    int             audio_diff_avg_count;
    
    
    double          audio_clock;
    double          frame_timer; 
    double          frame_last_pts;
    double          frame_last_delay;
    
    double          video_clock; 
    double          video_current_pts; 
    int64_t         video_current_pts_time;  
    
    //audio
    AVStream        *audio_st; //音频流
    AVCodecContext  *audio_ctx;//音频解码的上下文
    PacketQueue     audioq;//音频队列
    uint8_t         audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];//解码后的音频缓冲区
    unsigned int    audio_buf_size;//缓冲区的大小
    unsigned int    audio_buf_index;//现在已经使用了多少字节
    AVFrame         audio_frame;//解码后的音频帧
    AVPacket        audio_pkt;//解码之前的音频包
    uint8_t         *audio_pkt_data; //解码之前音频包的具体数据的指针
    int             audio_pkt_size;//解码之前音频包的具体数据的包的大小
    int             audio_hw_buf_size; // SDL音频缓冲区大小(单位字节)
    struct  SwrContext *audio_swr_ctx; //音频重采样上下文
    //video
    AVStream        *video_st; //视频的流
    AVCodecContext  *video_ctx;//视频的上下文
    PacketQueue     videoq;//视频流队列
    struct SwsContext *video_sws_ctx; //视频图像裁剪、缩放上下文

    
    VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE]; //解码后的视频帧队列
    int             pictq_size, pictq_rindex, pictq_windex;//解码后的视频流队列的大小、获取视频帧的位置、存放视频帧的位置
    
    SDL_mutex       *pictq_mutex; 
    SDL_cond        *pictq_cond; 
    
    SDL_Thread      *parse_tid;//解复用线程
    SDL_Thread      *video_tid;//解码线程
    
    int             quit; 
} VideoState;

enum 
{
    AV_SYNC_AUDIO_MASTER,
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_MASTER,
};




void QueueInit(PacketQueue *q) ;
int QueuePush(PacketQueue *q, AVPacket *pkt); 
int QueuePop(PacketQueue *q, AVPacket *pkt, int block);


#endif 
