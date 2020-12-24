#include "Audio.h"

double get_audio_clock(VideoState *is) 
{
    double pts;
    int hw_buf_size, bytes_per_sec, n;
    
    pts = is->audio_clock; 
    hw_buf_size = is->audio_buf_size - is->audio_buf_index; //音频缓冲区中还没有播放的数据(单位字节)
    bytes_per_sec = 0;
    n = is->audio_ctx->channels * 2;
    if(is->audio_st) 
    {
        bytes_per_sec = is->audio_ctx->sample_rate * n; //每秒钟播放的字节数
    }
    if(bytes_per_sec) 
    {
        pts -= (double)hw_buf_size / bytes_per_sec; 
    }

    return pts;
}


int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size, double *pts_ptr) 
{
    int len1, data_size = 0;
    AVPacket *pkt = &is->audio_pkt;
    double pts;
    int n;
    
    while(1)
    {
        while(is->audio_pkt_size > 0)
        {
            int got_frame = 0;
            //从解码之前音频包中取出packet进行解码
            len1 = avcodec_decode_audio4(is->audio_ctx, &is->audio_frame, &got_frame, pkt);
            if(len1 < 0) 
            {
                is->audio_pkt_size = 0;
                break;
            }
            data_size = 0;
            if(got_frame) 
            {                 
                data_size = 2 * is->audio_frame.nb_samples * 2;
                assert(data_size <= buf_size);
                
                swr_convert(is->audio_swr_ctx,
                            &audio_buf,
                            MAX_AUDIO_FRAME_SIZE*3/2,
                            (const uint8_t **)is->audio_frame.data,
                            is->audio_frame.nb_samples);
                
            }
            is->audio_pkt_data += len1;
            is->audio_pkt_size -= len1; 
            if(data_size <= 0) 
            {
                continue;   
            }
            pts = is->audio_clock;
            *pts_ptr = pts;
            n = 2 * is->audio_ctx->channels;
            is->audio_clock += (double)data_size /
            (double)(n * is->audio_ctx->sample_rate);
            return data_size;
        }
        if(pkt->data)
        {
            av_free_packet(pkt);
        }
            
        
        if(is->quit) 
        { 
            return -1;
        }
        if(QueuePop(&is->audioq, pkt, 1) < 0) 
        { 
            return -1;
        }
        is->audio_pkt_data = pkt->data;
        is->audio_pkt_size = pkt->size;
        
        if(pkt->pts != AV_NOPTS_VALUE) 
        {
            is->audio_clock = av_q2d(is->audio_st->time_base)*pkt->pts;
        }
    }
}
//回调函数，向SDL缓冲区填充数据
void audio_callback(void *userdata, Uint8 *stream, int len) 
{ 
    VideoState *is = (VideoState *)userdata;
    int len1, audio_size;
    double pts;
    
    SDL_memset(stream, 0, len);
    while(len > 0) 
    {
        if(is->audio_buf_index >= is->audio_buf_size) 
        {
            audio_size = audio_decode_frame(is, is->audio_buf, sizeof(is->audio_buf), &pts);
            if(audio_size < 0) 
            { 
                is->audio_buf_size = 1024 * 2 * 2;
                memset(is->audio_buf, 0, is->audio_buf_size); 
            } 
            else 
            {
                is->audio_buf_size = audio_size;
            }
            is->audio_buf_index = 0;
        }
        len1 = is->audio_buf_size - is->audio_buf_index;
        if(len1 > len)
        {
            len1 = len;
        }
            
        SDL_MixAudio(stream,(uint8_t *)is->audio_buf + is->audio_buf_index, len1, SDL_MIX_MAXVOLUME);
        len -= len1;
        stream += len1;
        is->audio_buf_index += len1;
    }
}


