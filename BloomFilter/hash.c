
#include "hash.h"
#include "ll.h"
#include "speck.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//
//  Constructor  for a HashTable.
//
//  length:   Length  of the  HashTable.
// salt:     Salt  for the  HashTable.
//
HashTable *ht_create(uint32_t length) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  if (ht) {
    ht->entries = 0;
    ht->salt[0] = 0x85ae998311115ae3;
    ht->salt[1] = 0xb6fac2ae33a40089;
    ht->length = length;
    ht->heads = (ListNode **)calloc(length, sizeof(ListNode *));

    return ht;
  }
  return (HashTable *)NIL;
}

//
//  Destructor  for a HashTable.
//
// ht:   The  HashTable.
//
void ht_delete(HashTable *ht) {
  int len = ht->length;
  for (int x = 0; x < len; x++) {
    if (ht->heads[x] != NULL) {
      ll_delete(ht->heads[x]); //using function to make sure all nodes are free
    } else {
      free(ht->heads[x]);
    }
  }
  free(ht->heads);
  free(ht);
}

//
//  Returns  number  of  entries  in hash  table
//
// h:   The  HashTable.
//
uint32_t ht_count(HashTable *h) {
  return h->entries;
}

//
//  Searches a HashTable  for a key.
//  Returns  the  ListNode  if found  and  returns  NULL  otherwise.
// This  should  call  the  ll_lookup ()  function.
//
// ht:       The  HashTable.
// key:      The key to  search  for.
//
ListNode *ht_lookup(HashTable *ht, char *key, bool move_to_front_rule) {
  int index2 = hash(ht->salt, key); //gets the index using the hash
  index2 = index2 % ht->length;

  ListNode *node = ll_lookup(
      ht->heads, key, index2, move_to_front_rule); //finds node using ll_lookup
  return node; //returns the node we found if any
}

//
// First  creates a new  ListNode  from  HatterSpeak.
// The  created  ListNode  is then  inserted  into a HashTable.
// This  should  call  the  ll_insert ()  function.
//
// ht:            The  HashTable.
// gs:            The  HatterSpeak  to add to the  HashTable.
//
void ht_insert(HashTable *ht, HatterSpeak *gs) {
  int index = hash(ht->salt, gs->oldspeak);
  index
      = index
        % ht->length; //the hash from the previous line is moded to fit in available indexs
  ll_insert(ht->heads, gs, index);
  ht->entries += 1;
}
