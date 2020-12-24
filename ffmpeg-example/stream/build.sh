#!/bin/bash

filename=$1
clang -Wall $1 -o ${filename%.*} -lavcodec -lavformat -lavfilter -lavutil -lswscale -lswresample -lSDL2
