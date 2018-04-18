CC=gcc
CFLAGS= -g -Wall -Werror -I.
LDFLAGS= -lm 

all: sparse

menu: sparse.o input.o compress.o sort.o

clean:
	@rm  *.o

clean-all: clean
	@rm -f menu
