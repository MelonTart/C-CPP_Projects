
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "word.h"

//
// Constructor for a word.
//
// syms:    Array of symbols a Word represents.
// len:     Length of the array of symbols.
// returns: Pointer to a Word that has been allocated memory.
//
Word *word_create(uint8_t *syms, uint32_t len) {
  Word *W = (Word *)malloc(sizeof(struct Word));
  W->syms = syms;
  W->len = len;
  return W;
}

//
// Constructs a new Word from the specified Word appended with a symbol.
// The Word specified to append to may be empty.
// If the above is the case, the new Word should contain only the symbol.
//
// w:       Word to append to.
// sym:     Symbol to append.
// returns: New Word which represents the result of appending.
//
Word *word_append_sym(Word *w, uint8_t sym) {
  uint8_t *NewStr; // creates a new array which will be the new word eventually
  NewStr = (uint8_t *)malloc(sizeof(uint8_t) * (w->len + 1));
  // NewStr[w->len + 1] = 0;//used as end of str thingy hopefully?
  for (int x = 0; x < (int)w->len; x++) {
    NewStr[x] = w->syms[x];
  }
  // printf("%s is the word were adding to \n",w->syms);
  // memcpy(NewStr,w->syms,w->len); //adding the word we are adding to
  NewStr[w->len] = sym;
  Word *New_word = word_create(NewStr, (w->len + 1));
  printf("");
  // printf("%s \n",NewStr); //need this or the code breaks for some reason ??
  return New_word;
}

//
// Destructor for a Word.
//
// w:       Word to free memory for.
// returns: Void.
//
void word_delete(Word *w) {
  if (w->len > 0) {
    free(w->syms);
  }
  // free(w->syms);
  free(w);
}

//
// Creates a new WordTable, which is an array of Words.
// A WordTable has a pre-defined size of MAX_CODE (UINT16_MAX - 1).
// This is because codes are 16-bit integers.
// A WordTable is initialized with a single Word at index EMPTY_CODE.
// This Word represents the empty word, a string of length of zero.
//
// returns: Initialized WordTable.
//
WordTable *wt_create(void) {
  WordTable *Wt = (WordTable *)calloc((UINT16_MAX - 1), sizeof(WordTable *));
  Wt[EMPTY_CODE] = word_create((uint8_t *)"", 0);
  return Wt;
}

//
// Resets a WordTable to having just the empty Word.
//
// wt:      WordTable to reset.
// returns: Void.
//
void wt_reset(WordTable *wt) {
  for (int x = 0; x < (UINT16_MAX - 1); x++) {
    wt[x]->syms = 0; // sets the word to nothing
    wt[x]->len = 0;
  }
}

//
// Deletes an entire WordTable.
// All Words in the WordTable must be deleted as well.
//
// wt:      WordTable to free memory for.
// returns: Void.
//
void wt_delete(WordTable *wt) {
  for (int x = 0; x < (UINT16_MAX - 1); x++) {
    if (wt[x] != NULL) {
      word_delete(wt[x]);
    }
  }
  free(wt);
}
