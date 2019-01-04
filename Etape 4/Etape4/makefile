CC=gcc
CFLAGS= -Wall -g

all : lecture

lecture: $(patsubst %.c, %.o, $(wildcard *.c) )
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o
