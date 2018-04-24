CC=gcc
CFLAGS= -g -pg -ansi -Wall -Werror -I.
LDFLAGS= -pg

all: main

main: main.o sparse.o compress.o sort.o el.o pos.o

clean:
	@rm  *.o

clean-all: clean
	@rm -f main
