After cloning the repository and navigating to the LZ78_Compression folder within a unix terminal,
typing make or make all should build the executable. The by typing ./encode or ./decode with
the given inputs on in the command line will run the program.
The inputs to our program are:
	“-i x” as a command line argument will set the input file to x, default stdin
	“-o x” as a command line argument will set the output file to x, default stdout
	“-v” as a command line argument will print statistics on the decompression/compression inlc: 
		Compressed/Decompressed file size, and the compression ratio

User input could be used by typing “< inputfile” after ./encode or ./decode ex:
./encode -i input.txt -o output.txt or ./encode -o output.txt < input.txt

Problems:
	Infer finds 5 dereferenced pointers and 1 "leak" however these are all pointers when get
	returned by the function they are in and are used in other portions of the code
	and are eventually free()'d as shown by running valgrind which shows 0 memory leaks
	Valgrind finds 1 error with encode and the file header being uninitalized however I do
	initalize it and it does write the correct magic number and file protection.

