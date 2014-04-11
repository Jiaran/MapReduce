CC = gcc
CFLAGS = -O2 
LDFLAGS = -pthread

OBJS = test.o csapp.o
#display.o

TEAM = NOBODY
VERSION = 1

all: test

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

#display.o: display.c
#	$(CC) $(CFLAGS) -I/usr/X11R6/include  -c display.c

test: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o test
#handin:
#	cp proxy.c $(INDIR)/$(TEAM)-$(VERSION)-proxy.c
#	cp csapp.c $(INDIR)/$(TEAM)-$(VERSION)-csapp.c
#	cp csapp.h $(INDIR)/$(TEAM)-$(VERSION)-csapp.h

clean:
	rm -f *~ csapp.o test test.o display.o core
