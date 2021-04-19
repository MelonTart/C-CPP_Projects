CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c99
CC = 	clang $(CFLAGS)

all		:	sorting

valgrind:
		valgrind ./sorting

sorting		:sorting.o bubble.o shell.o quick.o binary.o
		$(CC) -o sorting sorting.o bubble.o shell.o quick.o binary.o


sorting.o	:	sorting.c bubble.h shell.h quick.h binary.h
			$(CC) $(CFLAGS) -c sorting.c

shell.o : sorting.c shell.h
		$(CC) $(CFLAGS) -c shell.c
bubble.o 	: sorting.c bubble.h
		$(CC) $(CFLAGS) -c bubble.c
quick.o 	: sorting.c quick.h
		$(CC) $(CFLAGS) -c quick.c
binary.o 	: sorting.c binary.h
		$(CC) $(CFLAGS) -c binary.c

clean	:	
		rm -rf *.o infer-out

infer	:	
		make clean; infer run -- make; infer-capture -- make; infer-analyze -- make;
