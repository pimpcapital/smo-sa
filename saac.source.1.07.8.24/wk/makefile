CC=gcc

all : acwk

CFLAGS=-march=pentium4 -w -O3 -pipe -freg-struct-return -minline-all-stringops -mmmx -msse -msse2 $(INCFLAGS)

acwk : wk.c TCPIP.o error.o tools.o
	gcc -o acwk $(CFLAGS) wk.c error.o TCPIP.o tools.o

TCPIP:
error:
tools:

clean :
	rm -f acwk *.o *~