CFLAGS = -Wall -Wextra -Werror -Wpedantic
CC = 	clang $(CFLAGS)

all		:	encode decode

valgrind:
		valgrind ./encode

encode		:encode.o word.o io.o trie.o
		$(CC) -o encode encode.o word.o io.o trie.o
decode		:decode.o word.o io.o trie.o
		$(CC) -o decode decode.o word.o io.o trie.o


encode.o	:	encode.c word.h io.h trie.h code.h
			$(CC) $(CFLAGS) -c encode.c

word.o : encode.c decode.c word.h
		$(CC) $(CFLAGS) -c word.c
io.o 	: encode.c decode.c io.h code.h word.h
		$(CC) $(CFLAGS) -c io.c
trie.o 	: encode.c decode.c trie.h
		$(CC) $(CFLAGS) -c trie.c
clean	:	
		rm -rf *.o infer-out

infer	:	make clean; infer-capture -- make; infer-analyze -- make;
