OBJS = 
CC = g++
DEBUG = -g
CFLAGS =  -c $(DEBUG)
LFLAGS =  $(DEBUG) -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect 
 

all:  testJ filecenter

filestalk.o: filestalk.h filestalk.cpp
	$(CC) $(CFLAGS)  filestalk.cpp 

csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS)   csapp.c


filecenter:  filecenter.cpp csapp.o
	$(CC)  filecenter.cpp csapp.o -o filecenter -lpthread




testJ : master.cpp master.h worker.cpp worker.h testJ.cpp mapFunction.cpp reduceFunction.cpp  filestalk.o node.cpp csapp.o
	$(CC) master.cpp worker.cpp csapp.o node.cpp mapFunction.cpp reduceFunction.cpp testJ.cpp filestalk.o -o testJ -lpthread  $(LFLAGS)
