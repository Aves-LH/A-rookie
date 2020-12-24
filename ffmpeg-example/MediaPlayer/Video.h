/*************************************************************************
	> File Name: Video.h
	> Author: 
	> Mail: 
	> Created Time: 三 11/25 13:59:31 2020
 ************************************************************************/

#ifndef _VIDEO_H
#define _VIDEO_H

#include "PacketQueue.h"
#include "Audio.h"


extern VideoState *global_video_state;


void video_display(VideoState *is);
void alloc_picture(void *userdata);
int queue_picture(VideoState *is, AVFrame *pFrame, double pts);
double synchronize_video(VideoState *is, AVFrame *src_frame, double pts);
int decode_video_thread(void *arg);

#endif
