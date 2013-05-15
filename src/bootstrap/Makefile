CC=gcc
CFLAGS=-Wall -g -I..

all: bootstrap ../lst.img

../lst.img: bootstrap imageSource
	./bootstrap imageSource ../lst.img

bootstrap: bootstrap.o
	rm -f bootstrap
	$(CC) -o bootstrap bootstrap.o

# always clean up after yourself
clean:
	rm -f bootstrap.o bootstrap ../lst.img lst.img

distclean:
	rm -f bootstrap.o bootstrap ../lst.img lst.img