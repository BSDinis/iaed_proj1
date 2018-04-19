CC=gcc
CFLAGS= -g -Wall -Werror -I.
LDFLAGS= -lm 

all: test_pos test_el

test_pos: test_pos.o ../pos.o

test_el: test_el.o ../el.o ../pos.o

clean:
	@rm  *.o

clean-all: clean
	@rm -f test_pos test_el
