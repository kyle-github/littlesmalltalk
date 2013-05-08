#
#	Makefile for Little Smalltalk system
#	written by Tim Budd, Oregon State University, budd@cs.orst.edu
#

CC=gcc
CFLAGS=-O -g -Wall
# CFLAGS=-g -Wall -DDEBUG -DTRACE
# CFLAGS=-O -g -Wall -DPROFILE

# -lusr for profiling support in VSTa
#LIBS=-lusr
LIBS=

st: main.o interp.o memory.o
	rm -f st
	$(CC) $(CFLAGS) -o st main.o interp.o memory.o $(LIBS)
	
clean:
	rm -f *.o st

#
#	Makefile Notes
#
#	main.c contains three defined constants that may need to be changed
#
#	memory.h defines the main memory allocation routine (gcalloc) as
#	a macro -- this can be commented out and a procedure call will
#	be used instead -- slower but safer during debugging
