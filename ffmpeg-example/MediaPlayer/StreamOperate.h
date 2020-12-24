/*************************************************************************
	> File Name: StreamOperate.h
	> Author: 
	> Mail: 
	> Created Time: 三 11/25 14:22:41 2020
 ************************************************************************/

#ifndef _STREAMOPERATE_H
#define _STREAMOPERATE_H

#include "PacketQueue.h"
#include "Audio.h"
#include "Video.h"

//打开组件
int stream_component_open(VideoState *is, int stream_index);
//解复用线程
int demux_thread(void *arg);

#endif
