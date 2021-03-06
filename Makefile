# Filename: Amazing Project Makefile
# Description: The make file is to build up the query.
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -lcurl -ggdb -Wcpp
SRCDIR = src/
UTILDIR = ../util/src/
UTILFLAG = -ltseutil
UTILC=
UTILH= $(UTILDIR)amazing.h $(UTILDIR)utils.h 

#UTILLIB = $(UTILDIR)libtseutil.a 

# my project details
EXEC = AMStartup
OBJS = src/AMStartup.o
SRCS = src/AMStartup.c 
HDRS = ../util/src/amazing.h ../util/src/utils.h

all: AMStartup amazing_client


AMStartup: $(SRCDIR)AMStartup.o  $(SRCDIR)maze.o $(UTILLIB)
	$(CC) $(CFLAGS) -o AMStartup $(SRCDIR)AMStartup.o $(SRCDIR)maze.o

AMStartup.o: $(SRCDIR)AMStartup.c $(SRCDIR)AMStartup.h
	$(CC) $(CFLAGS) -c $(SRCDIR)AMStartup.c 

amazing_client: $(SRCDIR)amazing_client.o $(SRCDIR)maze.o $(UTILLIB) 
	$(CC) $(CFLAGS) -o amazing_client $(SRCDIR)amazing_client.o $(SRCDIR)maze.o

amazing_client.o: $(SRCDIR)amazing_client.c $(SRCDIR)amazing_client.h 
	$(CC) $(CFLAGS) -c $(SRCDIR)amazing_client.c 

maze.o: $(SRCDIR)maze.c $(UTILDIR)maze.h
	$(CC) $(CFLAGS) -c $(SRCDIR)maze.c

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make



clean:
	rm -f *~
	rm -f *#
	rm -f ./AMStartup
	rm -f ./amazing_client
	rm -f *.o
	rm -f src/*.o
	rm -f ./maze.log
