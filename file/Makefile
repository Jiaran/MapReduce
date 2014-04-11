CC = gcc
CFLAGS = -O2   
LDFLAGS = -pthread

OBJS = csapp.o filestalk.o 
#display.o

TEAM = NOBODY
VERSION = 1

all: filestalk

csapp.o: csapp.c 
	$(CC) $(CFLAGS) -c -g csapp.c

filestalk.o: filestalk.h filestalk.c
	$(CC) $(CFLAGS) -c -g filestalk.c


filestalk: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)  -o filetalk -lm
#handin:
#	cp proxy.c $(INDIR)/$(TEAM)-$(VERSION)-proxy.c
#	cp csapp.c $(INDIR)/$(TEAM)-$(VERSION)-csapp.c
#	cp csapp.h $(INDIR)/$(TEAM)-$(VERSION)-csapp.h

clean:
	rm -f *~ csapp.o chord chord.o display.o core
