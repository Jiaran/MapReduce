OBJS = main.o 
CC = g++
DEBUG = -g
CFLAGS =  -c $(DEBUG)
LFLAGS =  $(DEBUG) 

all: main testJ

main : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main
main.o : main.cpp constant.h
	$(CC) $(CFLAGS) main.cpp

testJ : master.cpp master.h worker.cpp worker.h testJ.cpp csapp.c node.cpp
	$(CC) master.cpp worker.cpp csapp.c node.cpp testJ.cpp -g  -o testJ -lpthread; 