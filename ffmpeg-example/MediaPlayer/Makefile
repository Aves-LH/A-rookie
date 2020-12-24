src=$(wildcard *.c)

all:MediaPlayer

CFLAGS = -Wall -lm -g -lavcodec -lavformat -lavutil -lavfilter -lswscale -lswresample -lSDL2

MediaPlayer:$(src)
	clang *.c -o $@ $(CFLAGS) -Wl,-rpath='./ffmpeg/lib'



