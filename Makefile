# Filename: Lab6 Query Makefile
# Description: The make file is to build up the query.
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -lcurl -ggdb
SRCDIR = /src/
UTILDIR = ../util/
UTILFLAG = -ltseutil
UTILLIB = $(UTILDIR)libtseutil.a 

# my project details
EXEC = AMStartup
OBJS = src/AMStartup.o
SRCS = src/AMStartup.c 
HDRS = ../util/src/amazing.h ../util/src/utils.h



#query:$(SRCS) $(HDRS) $(UTILLIB)
#	$(CC) $(CFLAGS) -o $@ $(SRCS) -L$(UTILDIR) $(UTILFLAG)

#$(UTILLIB): $(SRCS) $(HDRS)
#	cd $(UTILDIR); make;

AMStartup: $(SRCS) 
	$(CC) $(CFLAGS) -o AMStartup $(SRCS)

AMStartup.o: $(HDRS)
	$(CC) $(CFLAGS) -c src/AMStartup.c 




clean:
	rm -f *~
	rm -f *#
	rm -f ./AMStartup
	rm -f *.o
	rm -f src/*.o
#	cd $(UTILDIR); make clean;
