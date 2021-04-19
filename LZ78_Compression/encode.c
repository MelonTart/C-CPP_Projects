#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "code.h"
#include "io.h"
#include "trie.h"
#include "word.h"

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
  Fh.magic = MAGIC; // setting hte magic number
  Fh.protection = srcstats.st_mode;
  write_header(output, &Fh);

  // Compression Algo

  TrieNode *root = trie_create();
  TrieNode *curr_node = root;
  TrieNode *prev_node = NULL;
  uint8_t curr_sym = 0;
  uint8_t prev_sym = 0;

  uint16_t next_code = START_CODE;
  while (read_sym(input, &curr_sym)) {
    //  if(count<5000){
    //    printf("%c",curr_sym);
    //  }
    TrieNode *next_node = trie_step(curr_node, curr_sym);
    if (next_node != NULL) {
      prev_node = curr_node;
      curr_node = next_node;
    } else {

      buffer_pair(output, curr_node->code, curr_sym, natLog(next_code));
      curr_node->children[curr_sym] = trie_node_create(next_code);
      curr_node = root;
      next_code = next_code + 1;
    }
    if (next_code == MAX_CODE) {
      // printf("got to max code\n");
      trie_reset(root);
      curr_node = root;
      next_code = START_CODE;
    }
    prev_sym = curr_sym;
  }
  if (curr_node != root) {
    buffer_pair(output, prev_node->code, prev_sym, natLog(next_code));
    next_code = (next_code + 1) % MAX_CODE;
  }
  buffer_pair(output, STOP_CODE, 0, natLog(next_code));
  flush_pairs(output);

  close(input);
  close(output);

  trie_delete(root);

  if (verboseness) {
    print_Stats(1);
  }

  return 0;
}
