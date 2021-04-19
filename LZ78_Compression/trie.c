
#include "trie.h"
#include "code.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

#define ALPHABET 256

typedef struct TrieNode TrieNode;

//
// Constructor for a TrieNode.
//
// index:   Index of the constructed TrieNode.
// returns: Pointer to a TrieNode that has been allocated memory.
//
TrieNode *trie_node_create(uint16_t index) {
  struct TrieNode *T = (struct TrieNode *)malloc(sizeof(struct TrieNode));
  T->code = index;
  for (int x = 0; x < ALPHABET; x++) {
    T->children[x] = NULL;
  }
  return T;
}

//
// Destructor for a TrieNode.
//
// n:       TrieNode to free allocated memory for.
// returns: Void.
//
void trie_node_delete(TrieNode *n) { free(n); }

//
// Initializes a Trie: a root TrieNode with the index EMPTY_CODE.
//
// returns: Pointer to the root of a Trie.
//
TrieNode *trie_create(void) {
  TrieNode *T = trie_node_create(EMPTY_CODE);
  return T;
}

//
// Resets a Trie to just the root TrieNode.
//
// root:    Root of the Trie to reset.
// returns: Void.
//
void trie_reset(TrieNode *root) {
  for (int x = 0; x < ALPHABET; x++) {
    if (root->children[x] != NULL) { // check all the children and if they
                                     // exist, remove them and their children
      trie_reset(
          root->children[x]); // recursively remove everything but the root
      trie_node_delete(root->children[x]);
    }
  }
}

//
// Deletes a sub-Trie starting from the sub-Trie's root.
//
// n:       Root of the sub-Trie to delete.
// returns: Void.
//
void trie_delete(TrieNode *n) {
  for (int x = 0; x < ALPHABET; x++) {
    if (n->children[x] != NULL) {  // check all the children and if they exist,
                                   // remove them and their children
      trie_delete(n->children[x]); // recursively remove everything
    }
  }
  trie_node_delete(n); // everynode it goes through will be deleted after
                       // looping even the root
}

//
// Returns a pointer to the child TrieNode reprsenting the symbol sym.
// If the symbol doesn't exist, NULL is returned.
//
// n:       TrieNode to step from.
// sym:     Symbol to check for.
// returns: Pointer to the TrieNode representing the symbol.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
  if (n->children[sym] != NULL) {
    return n->children[sym];
  } else {
    return NULL;
  }
}
