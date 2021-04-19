CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c99
CC = 	clang $(CFLAGS)

all		:	hatterspeak

valgrind:
		valgrind ./hatterspeak

hatterspeak		:hatterspeak.o speck.o parser.o hash.o ll.o bf.o bv.o
		$(CC) -o hatterspeak hatterspeak.o speck.o parser.o hash.o ll.o bf.o bv.o


hatterspeak.o	:	hatterspeak.c speck.h parser.h hash.h ll.h bf.h bf.h bv.h
			$(CC) $(CFLAGS) -c hatterspeak.c

parser.o : hatterspeak.c parser.h
		$(CC) $(CFLAGS) -c parser.c
speck.o 	: hatterspeak.c speck.h
		$(CC) $(CFLAGS) -c speck.c
hash.o 	: hatterspeak.c hash.h speck.h ll.h
		$(CC) $(CFLAGS) -c hash.c
ll.o 	: hatterspeak.c ll.h 
		$(CC) $(CFLAGS) -c ll.c
bv.o 	: hatterspeak.c bv.h
		$(CC) $(CFLAGS) -c bv.c

bf.o 	: hatterspeak.c bf.h bv.h speck.h
		$(CC) $(CFLAGS) -c bf.c

clean	:	
		rm -rf *.o infer-out

infer	:	make clean; infer-capture -- make; infer-analyze -- make;
