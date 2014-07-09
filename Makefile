CFLAGS=`pkg-config --cflags Wand` -g3 -O3
LDLIBS=`pkg-config --libs Wand` -lpthread -lm -ldl 

ascsee: ascsee.o
clean:
	rm ascsee.o ascsee

