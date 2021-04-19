
#include "bf.h"
#include "bv.h"
#include "speck.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

//
//  Constructor  for a BloomFilter.
//
// size:   The  number  of  entries  in the  BloomFilter.
//
BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->primary[0] = 0xfc28ca6885711cf7;
    bf->primary[1] = 0x2841af568222f773;
    bf->secondary[0] = 0x85ae998311115ae3;
    bf->secondary[1] = 0xb6fac2ae33a40089;
    bf->tertiary[0] = 0xd37b01df0ae8f8d0;
    bf->tertiary[1] = 0x911d454886ca7cf7;
    bf->filter = bv_create(size);
    bf->filter->length = size;
    return bf;
  }
  return (BloomFilter *)NIL;
}

//
//  Destructor  for a BloomFilter.
//
// bf:   The  BloomFilter.
//
void bf_delete(BloomFilter *bf) {
  bv_delete(bf->filter);
  free(bf);
}

//
//  Inserts a new  key  into  the  BloomFilter.
//  Indices  to set  bits  are  given by the   hash  functions.
//
// bf:     The  BloomFilter.
// key:    The  key to  insert  into  the  BloomFilter.
//
void bf_insert(BloomFilter *bf, char *key) {
  int index1 = hash(bf->primary, key) % bf->filter->length;
  int index2 = hash(bf->secondary, key) % bf->filter->length;
  int index3
      = hash(bf->tertiary, key)
        % bf->filter->length; //finds the 3 indexs and then sets their bits
  bv_set_bit(bf->filter, index1);
  bv_set_bit(bf->filter, index2);
  bv_set_bit(bf->filter, index3);
}

//
// bf:     The  BloomFilter.
// key:    The  key in which to check  membership.
//
bool bf_probe(BloomFilter *bf, char *key) {
  int index1 = hash(bf->primary, key) % bf->filter->length;
  int index2 = hash(bf->secondary, key) % bf->filter->length;
  int index3 = hash(bf->tertiary, key) % bf->filter->length;
  bool bit1 = bv_get_bit(bf->filter, index1);
  bool bit2 = bv_get_bit(
      bf->filter, index2); //finds the 3 indexs and then gets their bits
  bool bit3 = bv_get_bit(bf->filter, index3);
  if (bit1 && bit2 && bit3) {
    return 1;
  } else {
    return 0;
  }
}
