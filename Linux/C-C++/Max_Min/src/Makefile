src=$(wildcard *.cpp)
obj=$(patsubst %.cpp,%.obj,$(src) )
objdir=../obj
outdir=../out

target=Main.out
#CC=g++
CFLAGS= -c -Wall -g

all:$(objdir) $(outdir) $(target)

$(objdir):
	-mkdir $@

$(outdir):
	-mkdir $@

$(target):$(obj)
	g++ $(objdir)/* -o $(outdir)/$@
%.obj:%.cpp
	g++ $(CFLAGS) $< -o $(objdir)/$@
	
clean:
	-rm -rf $(target) $(obj) $(objdir)


test:
	@echo $(src)
	@echo $(obj)


#Main.out:Max_Min.obj Main.obj
#	g++ Max_Min.obj Main.obj -o Main.out
#Main.obj:Main.cpp
#	g++ -c Main.cpp -o Main.obj
#Max_Min.obj:Max_Min.cpp
#	g++ -c Max_Min.cpp -o Max_Min.obj

#clean:
#	-rm -rf Max_Min.obj Main.obj

#test:
#	@echo Max_Min.obj Main.obj
