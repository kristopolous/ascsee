CFLAGS=`pkg-config --cflags Wand` -g3 
LDLIBS=`pkg-config --libs Wand` -lpthread -lm -ldl 

ascsee: ascsee.o

