	
To make the file copy the repo, navagate to the BloomFilter folder in a unix client and the type make or
	make all. The program can then be run by typing ./hatterspeak optionally followed by the command
	line agruments below 
The inputs to our program are:
“-h x” as a command line argument will set the hash table size to x, default 10000
“-f x” as a command line argument will set the bloom filter size to x, default 220
“-m” as a command line argument will use the move to front rule where words that are searched, 
	get moved to the front of the linked list for future searches
“-b” as a command line argument will not use the move to front rule described in 3
“-s” as a command line argument will suppress the letter from the censor and instead print 
	statistics such as, seeks, avg seek length, avg linked list length, hash table load, 
	and bloom filter load

Required User input should be used by typing “< inputfile” after ./hatterspeak ex:
	./hatterspeak -f 1000 -h 1000 < input.in
	// input.in can be changed out for any text file
