CC=gcc
CFLAGS=-I.

all: reversi

reversi: main.o reversi.o 
   $(CC) -o reversi main.o reversi.o 

main.o: main.c
   $(CC) -c -o main.o main.c

reversi.o: reversi.c
   $(CC) -c -o reversi.o reversi.c

clean:
   rm -f *.o reversi
