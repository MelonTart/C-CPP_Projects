CFLAGS = -Wall -Wextra -Werror -Wpedantic
CC = 	clang $(CFLAGS)

all		:	sequence

sequence		:bv.o sequence.o sieve.o
		$(CC) -o sequence bv.o sequence.o sieve.o

sequence.o	:	sequence.c	bv.h sieve.h
			$(CC) $(CFLAGS) -c sequence.c
sieve.o : sequence.c sieve.h  bv.h
		$(CC) $(CFLAGS) -c sieve.c
bv.o 	: sequence.c bv.h sieve.h
		$(CC) $(CFLAGS) -c bv.c
clean	:	
		rm -rf sequence sequence.o sieve.o bv.o infer-out

infer	:	
		make clean; infer-capture -- make; infer-analyze -- make
