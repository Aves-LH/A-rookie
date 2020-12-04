#include "PacketQueue.h"
#include "Video.h"

int screen_left = SDL_WINDOWPOS_CENTERED;
int screen_top = SDL_WINDOWPOS_CENTERED;
int screen_width = 0;
int screen_height = 0;
int resize = 1;

void QueueInit(PacketQueue *q) 
{
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

//向链表队列尾部插入节点
int QueuePush(PacketQueue *q, AVPacket *pkt) 
{
    
    AVPacketList *pkt1;
    if(av_dup_packet(pkt) < 0) 
    {
        return -1;
    }
    pkt1 = (AVPacketList *)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
    {
        return -1;
    }
    pkt1->pkt = *pkt;
    pkt1->next = NULL;
    
    SDL_LockMutex(q->mutex);
    
    if (!q->last_pkt)
    {
        q->first_pkt = pkt1;
    }  
    else
    {
        q->last_pkt->next = pkt1; 
    }
    q->last_pkt = pkt1; 
    q->nb_packets++; 
    q->size += pkt1->pkt.size; 
    SDL_CondSignal(q->cond); 
    
    SDL_UnlockMutex(q->mutex);
    return 0;
}

//取出队列中的首节点
int QueuePop(PacketQueue *q, AVPacket *pkt, int block)
{
    AVPacketList *pkt1;
    int ret;
    
    SDL_LockMutex(q->mutex);
    
    while(1)
    {
        if(global_video_state->quit)
        {
            ret = -1;
            break;
        }
        
        pkt1 = q->first_pkt;
        if (pkt1) 
        {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
            {
                q->last_pkt = NULL;
            }   
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } 
        else if (!block) 
        {
            ret = 0;
            break;
        } 
        else 
        {
            SDL_CondWait(q->cond, q->mutex);//等待
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

