CC=gcc
CFLAGS= -g -Wall -Werror -I.
LDFLAGS= -lm 

all: matrix

menu: matrix.o input.o #compress.o book.o

clean:
	@rm  *.o

clean-all: clean
	@rm -f menu
