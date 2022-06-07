src=$(wildcard *.c)
obj=$(patsubst %.c,%.o,$(src) )
target=jpeg.out

CC=gcc
CFLAGS= -c -Wall -g -I/home/aves/jpeg-8b/
LFLAGS=-ljpeg -L/home/aves/jpeg-8b/.libs/

$(target):$(obj)
	$(CC) $^ -o $@ $(LFLAGS)
%.o:%.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf $(target) $(obj)
