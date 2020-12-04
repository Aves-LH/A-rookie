#!/bin/bash
clang Audio.c MediaPlayer.c StreamOperate.c PacketQueue.c Video.c -o MediaPlayer -lavcodec -lavformat -lavutil -lswscale -lswresample -lSDL2

