#
#	Makefile for Little Smalltalk system
#	written by Tim Budd, Oregon State University, budd@cs.orst.edu
#
# Modified by Andy Valencia for VSTa.
# 
# Modified by Kyle Hayes 2001, 2002, 2003, 2013
#

CC=gcc
CFLAGS=-Os -g -Wall
# CFLAGS=-g -Wall -DDEBUG -DTRACE
# CFLAGS=-O -g -Wall -DPROFILE

# -lusr for profiling support in VSTa
#LIBS=-lusr
LIBS=

all=../lst

../lst: main.o interp.o memory.o
	rm -f ../lst
	$(CC) $(CFLAGS) -o ../lst main.o interp.o memory.o $(LIBS)
	
interp.o: interp.c globs.h interp.h memory.h
	$(CC) -c -o interp.o $(CFLAGS) interp.c
	
main.o: main.c globs.h interp.h memory.h
	$(CC) -c -o main.o $(CFLAGS) main.c
	
memory.o: memory.c memory.h globs.h
	$(CC) -c -o memory.o $(CFLAGS) memory.c
	
clean:
	rm -f *.o lst ../lst *~

distclean:
	rm -f *.o lst ../lst *~

#
#	Makefile Notes
#
#	main.c contains three defined constants that may need to be changed
#
#	memory.h defines the main memory allocation routine (gcalloc) as
#	a macro -- this can be commented out and a procedure call will
#	be used instead -- slower but safer during debugging
