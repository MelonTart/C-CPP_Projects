#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"
#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

uint16_t natLog(uint16_t number) {
  float div = number; // used to find the length the code should be
  uint16_t count = 0;
  while (div >= 1) {
    div = div / 2;
    count += 1;
  }
  return count;
}

int main(int argc, char **argv) {

  char *InputFile;
  char *OutputFile;
  int verboseness = 0;
  int giveninputfile = 0;
  int givenoutputfile = 0;

  for (int i = 0; i < argc; i++) { // loop thorugh command line arguments
    if (strcmp(argv[i], "-v") == 0) {
      verboseness = 1;
    }
    if (strcmp(argv[i], "-i") == 0) {
      InputFile = (char *)argv[i + 1];
      giveninputfile = 1;
    }
    if (strcmp(argv[i], "-o") == 0) {
      OutputFile = (char *)argv[i + 1];
      givenoutputfile = 1;
    }
  }

  int input;
  int output;

  if (giveninputfile) {
    input = open(InputFile, O_RDWR | O_CREAT, 0600);
  } else {
    input = 0; // 0 is stdin
  }
  if (givenoutputfile) {
    output = open(OutputFile, O_RDWR | O_CREAT | O_TRUNC, 0600);
  } else {
    output = 1; // 1 is stdout
  }

  if (input == -1 || output == -1) {
    printf("Sorry there was a problem opening your files\n");
    return 0;
  }

  struct stat srcstats;

  fstat(input, &srcstats);
  fchmod(output,
         srcstats.st_mode); // copy permissions from inputfile to output file
         
  FileHeader Fh;

  read_header(input, &Fh);

  // printf("%u is the magic number and\n%hu is the
  // protection\n",Fh.magic,Fh.protection);

  if (Fh.magic != 2343419631) {
    printf("Sorry your file seemed to lack a little *magic* I would suggest "
           "going back the way you came\n");
    return 0;
  }

  WordTable *table = wt_create();
  uint8_t curr_sym = 0;
  uint16_t curr_code = 0;
  uint16_t next_code = START_CODE;
  while (read_pair(input, &curr_code, &curr_sym, natLog(next_code))) {
    // printf("%d ",curr_code);
    // printf("(code %d ,sym %c)\n",curr_code,curr_sym);
    table[next_code] = word_append_sym(table[curr_code], curr_sym);
    buffer_word(output, table[next_code]);
    next_code += 1;
    if (next_code == MAX_CODE) {
      wt_reset(table);
      next_code = START_CODE;
    }
  }
  // printf("(code %d ,sym %c) final\n",curr_code,curr_sym);
  flush_words(output);

  close(input);
  close(output);
  wt_delete(table);

  if (verboseness) {
    print_Stats(0);
  }

  return 0;
}
