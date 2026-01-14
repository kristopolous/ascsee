CFLAGS=-I/usr/include/ImageMagick-7/ -g3 -O3
LDLIBS=-lGraphicsMagick -lpthread -lm -ldl 

ascsee: ascsee.o
clean:
	rm ascsee.o ascsee

