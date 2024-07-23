CFLAGS=`pkg-config --cflags MagickWand` -g3 -O3
LDLIBS=`pkg-config --libs MagickWand` -lpthread -lm -ldl 

ascsee: ascsee.o
clean:
	rm ascsee.o ascsee

