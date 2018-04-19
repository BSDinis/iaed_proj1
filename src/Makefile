CC=gcc
CFLAGS= -g -Wall -Werror -I.
LDFLAGS= -lm 

all: main

main: main.o sparse.o read_write.o compress.o sort.o el.o pos.o

clean:
	@rm  *.o

clean-all: clean
	@rm -f main
