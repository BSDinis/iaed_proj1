CC=gcc
CFLAGS= -g -ansi -Wall -Werror -I.
LDFLAGS= -lm 

all: main

main: main.o sparse.o compress.o sort.o el.o pos.o

clean:
	@rm  *.o

clean-all: clean
	@rm -f main
