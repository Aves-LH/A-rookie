src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

ALL:Main.out

Main.out:$(obj)
	gcc -Wall $^ -o $@

$(obj):%.o:%.c
	gcc -c $< -o $@

clean:
	-rm -rf $(obj)

.PHONY:clean ALL
