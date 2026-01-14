CC = gcc
CFLAGS = $(shell pkg-config --cflags ImageMagick) -g3 -O3
LDLIBS = $(shell pkg-config --libs MagickWand) -lpthread -lm -ldl 

ascsee: ascsee.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


clean:
	rm ascsee.o ascsee

