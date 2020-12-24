/*************************************************************************
	> File Name: Audio.h
	> Author: 
	> Mail: 
	> Created Time: 三 11/25 14:12:53 2020
 ************************************************************************/

#ifndef _AUDIO_H
#define _AUDIO_H

#include "PacketQueue.h"
#include "Video.h"

double get_audio_clock(VideoState *is); 
int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size, double *pts_ptr); 
void audio_callback(void *userdata, Uint8 *stream, int len); 

#endif
