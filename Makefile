#this is makefile
Main.out:Main.o MyString.o CmdProc.o
	g++ Main.o MyString.o CmdProc.o -o Main.out
Main.o:Main.cpp CmdProc.cpp MyString.cpp
	g++ Main.cpp CmdProc.cpp MyString.cpp -o Main.o
MyString.o:MyString.cpp
	g++ MyString.cpp -o MyString.o
CmdProc.o:CmdProc.cpp
	g++ CmdProc.cpp -o CmdProc.o
