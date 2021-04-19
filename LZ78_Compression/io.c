
#include "io.h"
#include "code.h"
#include "word.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

uint64_t total_syms = 0;
uint64_t total_read = 0;
uint64_t total_written = 0;

uint64_t binary_index = 0;
uint64_t character_index = 0;

//
// Wrapper for the read() syscall.
// Loops to read the specified number of bytes, or until input is exhausted.
// Returns the number of bytes read.
//
// infile:  File descriptor of the input file to read from.
// buf:     Buffer to store read bytes into.
// to_read: Number of bytes to read.
// returns: Number of bytes read.
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
  int c;
  int bytes_read = 0;

  while (((c = read(infile, (void *)(buf + bytes_read), 1)) != 0) &&
         bytes_read != to_read) {
    // if(bytes_read<10){
    //     printf("%c\n",*(buf+bytes_read));
    //     printf("%d bytes read\n",c);
    // }

    bytes_read += 1;
  }
  total_read += bytes_read;
  return bytes_read;
}

//
// Wrapper for the write() syscall.
// Loops to write the specified number of bytes, or until nothing is written.
// Returns the number of bytes written.
//
// outfile:   File descriptor of the output file to write to.
// buf:       Buffer that stores the bytes to write out.
// to_write:  Number of bytes to write.
// returns:   Number of bytes written.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {

  int c;
  int bytes_written = 0;

  while (((c = write(outfile, (void *)(buf + bytes_written), 1)) != 0) &&
         bytes_written != to_write) {
    // moving the buffer to read to the next space
    bytes_written += 1;
  }
  total_written += bytes_written;
  return bytes_written;
}
//
// Reads in a FileHeader from the input file.
//
// infile:  File descriptor of input file to read header from.
// header:  Pointer to memory where the bytes of the read header should go.
// returns: Void.
//
void read_header(int infile, FileHeader *header) {
  read(infile, header, sizeof(header)); // reads in the header object
  total_read += sizeof(header);
}

//
// Writes a FileHeader to the output file.
//
// outfile: File descriptor of output file to write header to.
// header:  Pointer to the header to write out.
// returns: Void.
//
void write_header(int outfile, FileHeader *header) {
  int c;
  c = write(outfile, header, sizeof(header));
  total_written += sizeof(header);
}

static uint8_t word_buf[4000];

static int EndOfBuff = 3999;

void clear_word_buff() {
  for (int x = 0; x < 4000; x++) {
    word_buf[x] = 0;
  }
}

static int SymIndex = 0;

static int IndexOfBuffer = 0;
//
// "Reads" a symbol from the input file.
// The "read" symbol is placed into the pointer to sym (pass by reference).
// In reality, a block of symbols is read into a buffer.
// An index keeps track of the currently read symbol in the buffer.
// Once all symbols are processed, another block is read.
// If less than a block is read, the end of the buffer is updated.
// Returns true if there are symbols to be read, false otherwise.
//
// infile:  File descriptor of input file to read symbols from.
// sym:     Pointer to memory which stores the read symbol.
// returns: True if there are symbols to be read, false otherwise.
//

bool read_sym(int infile, uint8_t *sym) {
  int c;
  if (IndexOfBuffer == 0 ||
      IndexOfBuffer ==
          4000) { // if it just has been used or needs be used again
    SymIndex =
        0; // reset the index so it can read the first value of the new block
    // printf("\n%c was the last letter\n",word_buf[3999]);
    clear_word_buff();
    c = read_bytes(infile, word_buf, 3999); // read in 4000 symbols
    // printf("\n%c is the first letter of the new block\n",word_buf[0]);
    // printf("%s",word_buf);
    EndOfBuff = c; // the amount of letters is the number of bytes read in
    IndexOfBuffer = 0;
  }
  if (IndexOfBuffer ==
      EndOfBuff + 1) { // means we ran out of data to read as end of buff =!
                       // 4000 and we are at the last index
    return 0;
  }

  *sym = word_buf[IndexOfBuffer]; // sets sym to the next symbol and increments
                                  // the index
  IndexOfBuffer = IndexOfBuffer + 1;
  return 1; // not at end of file so it continues
}

//
// Buffers a pair. A pair is comprised of a symbol and an index.
// The bits of the symbol are buffered first, starting from the LSB.
// The bits of the index are buffered next, also starting from the LSB.
// bit_len bits of the index are buffered to provide a minimal representation.
// The buffer is written out whenever it is filled.
//
// outfile: File descriptor of the output file to write to.
// sym:     Symbol of the pair to buffer.
// index:   Index of the pair to buffer.
// bit_len: Number of bits of the index to buffer.
// returns: Void.
//

static uint8_t binarybuf[4000];
void clearBuffer() {
  for (int x = 0; x < 32000; x++) {
    binarybuf[x / 8] &=
        ~(1 << (x % 8)); // used to clear the buffer once it fills up
  }
}

void buffer_pair(int outfile, uint16_t code, uint8_t sym, uint8_t bit_len) {
  // buffer the sym

  // printf("(code %d ,sym %c) is the pair being buffered \n",code,sym);

  // printf("%c",(char)sym);
  for (uint8_t x = 0; x < 8; x++) {
    if ((sym & (1 << (x % 8))) >
        0) { // if it should set the bit bc the code has the bit set
      binarybuf[binary_index / 8] |=
          1 << (binary_index % 8); // setting the bit at binary index
    }

    binary_index++; // moving the index up to either set it to one or not

    if (binary_index == 32000) {
      // printf("had to write a block and restart\n");
      write_bytes(outfile, binarybuf,
                  4000); // if everybit is used, write out the binary buffer
      // printf("%s",binarybuf);
      binary_index = 0;
      clearBuffer();
    }
  }
  // printf(" is the binary incoding of it\n");
  // buffer the code

  for (uint16_t x = 0; x < bit_len; x++) {
    if ((code & (1 << (x % 16))) >
        0) { // if it should set the bit bc the code has the bit set
      binarybuf[binary_index / 8] |=
          1 << (binary_index % 8); // setting the bit at binary index
    }
    // printf("1");
    // }else{
    //   printf("0");
    // }

    binary_index++;

    if (binary_index == 32000) {
      // printf("had to write a block and restart\n");
      write_bytes(outfile, binarybuf,
                  4000); // if everybit is used, write out the binary buffer
      // printf("%s",binarybuf);
      binary_index = 0;
      clearBuffer();
    }
  }
  // printf(" \n");
}

//
// Writes out any remaining pairs of symbols and indexes to the output file.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_pairs(int outfile) {
  int bytes_to_flush;
  if (binary_index % 8 != 0) { // if it doesnt fit into a rounded number of
                               // bytes
    bytes_to_flush = binary_index / 8 + 1;
  } else {
    bytes_to_flush = binary_index / 8;
  }

  write_bytes(
      outfile, binarybuf,
      bytes_to_flush); // if everybit is used, write out the binary buffer
  binary_index = 0;
  total_written += 1; // doesnt count the last one so i have to add it
  clearBuffer();
}

//
// "Reads" a pair (symbol and index) from the input file.
// The "read" symbol is placed in the pointer to sym (pass by reference).
// The "read" index is placed in the pointer to index (pass by reference).
// In reality, a block of pairs is read into a buffer.
// An index keeps track of the current bit in the buffer.
// Once all bits have been processed, another block is read.
// The first 8 bits of the pair constitute the symbol, starting from the LSB.
// The next bit_len bits constitutes the index, starting from the the LSB.
// Returns true if there are pairs left to read in the buffer, else false.
// There are pairs left to read if the read index is not STOP_INDEX.
//
// infile:  File descriptor of the input file to read from.
// sym:     Pointer to memory which stores the read symbol.
// index:   Pointer to memory which stores the read index.
// bit_len: Length in bits of the index to read.
// returns: True if there are pairs left to read, false otherwise.
//
static int bufferIndex = 0; // used to read not write
                            //*(&code+ 2/8) |= 1 << (2 % 8);

uint8_t squareNum(uint8_t n) {
  uint8_t num = 1;
  for (uint8_t x = 0; x < n; x++) {
    num = num * 2;
  }
  return num;
}

uint16_t squareNum2(uint16_t n) {
  uint16_t num = 1;
  for (uint16_t x = 0; x < n; x++) {
    num = num * 2;
  }
  return num;
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, uint8_t bit_len) {

  if (bufferIndex == 0) { // if it just has been used or needs be used again
    clearBuffer();
    read_bytes(infile, binarybuf, 4000);
  }

  // for(int x =0;x<8;x++){
  //     if( ( binarybuf[(x+bufferIndex)/8] & (1 << ((x+bufferIndex) % 8))) > 0
  //     ){
  //         printf("1");
  //     }else{
  //         printf("0");
  //     }
  // }

  // reading in sym
  int start_index = bufferIndex;
  uint8_t value = 0;
  uint8_t offset = 0; // used incase it has to reload the buffer
  for (uint8_t x = 0; x < 8; x++) {
    if ((binarybuf[(x + start_index - offset) / 8] &
         (1 << ((x + start_index - offset) % 8))) >
        0) { // if it should set the bit bc the code has the bit set
      value += squareNum(x);
    }
    bufferIndex += 1;
    if (bufferIndex == 32000) { // if it needs to load in more
      // printf("buffer filledUP!\n");
      offset = x + 1;
      clearBuffer();
      read_bytes(infile, binarybuf, 4000);
      bufferIndex = 0;
      start_index = 0;
    }
  }

  // printf("The value is, %d",value);
  *sym = value;

  start_index = bufferIndex; // moving the start index up

  // reading in code
  uint16_t code_value = 0;
  offset = 0;

  for (uint16_t x = 0; x < (uint16_t)bit_len; x++) {
    if ((binarybuf[(x + start_index - offset) / 8] &
         (1 << ((x + start_index - offset) % 8))) >
        0) { // if it should set the bit bc the code has the bit set
      code_value += squareNum2(x);
    }
    bufferIndex += 1;
    if (bufferIndex == 32000) { // if it needs to load in more
      offset = x + 1;
      clearBuffer();
      read_bytes(infile, binarybuf, 4000);
      bufferIndex = 0;
      start_index = 0;
    }
  }
  // printf("code value %d  ",code_value);
  *code = code_value;
  if (code_value == STOP_CODE) {
    // printf(" was stopped at binary index %d  ",bufferIndex);
    return 0;
  } else {
    return 1;
  }
}

//
// Buffers a Word, or more specifically, the symbols of a Word.
// Each symbol of the Word is placed into a buffer.
// The buffer is written out when it is filled.
//
// outfile: File descriptor of the output file to write to.
// w:       Word to buffer.
// returns: Void.
//

static uint16_t word_index = 0; // used to store words

void buffer_word(int outfile, Word *w) {
  for (uint32_t x = 0; x < w->len; x++) {
    *(word_buf + word_index) = w->syms[x]; // setting the next word in the word
    // printf("adding %c to the buffer\n",w->syms[x]);
    word_index += 1;
    if (word_index == 4000) {
      // printf("There were more than 4000 letters so we go again\n");
      // printf("\n%c is the last letter?\n",word_buf[3999]);
      write_bytes(outfile, word_buf, 4000 - 1);
      clear_word_buff();
      word_index = 0;
    }
  }
  // printf("%s is the message so far\n",word_buf);
}

//
// Writes out any remaining symbols in the buffer.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_words(int outfile) {
  // printf("is the number of letters %d \n",word_index);
  // printf("%s\n",word_buf);
  // printf("%c is the first letter?\n",word_buf[0]);
  write_bytes(outfile, word_buf, word_index - 2);
  total_written += 1; // doesnt count the last one so i have to add it
  clear_word_buff();
  word_index = 0;
}

void print_Stats(
    int encode) { // encode is if this is encode printing the stats or not
  if (encode) {
    float ratio = 100 * (1 - ((float)total_written / (float)total_read));
    printf("Compressed  file  size: %lu bytes\nUncompressed  file  size: %lu "
           "bytes\nCompression  ratio: %2.2f\n",
           total_written, total_read, ratio);
  } else {
    float ratio = 100 * (1 - ((float)total_read / (float)total_written));
    printf("Compressed  file  size: %lu bytes\nUncompressed  file  size: %lu "
           "bytes\nCompression  ratio: %2.2f\n",
           total_read, total_written, ratio);
  }
}
