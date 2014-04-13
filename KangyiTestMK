OBJS = main.o 
CC = g++
DEBUG = -g
CFLAGS =  -c $(DEBUG)
LFLAGS =  $(DEBUG) 
FILEOBJS = csapp.o filestalk.o 

all: main testJ filestalk

main : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp


filestalk.o: filestalk.h filestalk.cpp
	$(CC) $(CFLAGS)  -g filestalk.cpp 

csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS)  -g csapp.c


filestalk: $(FILEOBJS)
	$(CC) $(FILEOBJS) -o filestalk -lpthread




testJ : master.cpp master.h worker.cpp worker.h testJ.cpp  node.cpp csapp.c
	$(CC) master.cpp worker.cpp csapp.o node.cpp testJ.cpp -g  -o testJ -lpthread; 