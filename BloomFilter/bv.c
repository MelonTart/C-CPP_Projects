#include "bv.h"
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

BitVector *bv_create(uint32_t bit_len) {
  struct BitVector *BitVector
      = (struct BitVector *)malloc(sizeof(struct BitVector));
  BitVector->length = bit_len;
  BitVector->vector = (uint8_t *)malloc((BitVector->length / 8));
  BitVector->setBits = 0;
  return BitVector;
}
void bv_delete(BitVector *v) {
  free(v->vector);
  free(v);
}
uint32_t bv_get_len(BitVector *v) {
  return v->length;
}
void bv_set_bit(BitVector *v, uint32_t i) {
  //v->vector[i] = 1;
  if (bv_get_bit(v, i)) { //if the bit is already set
    return;
  } else {
    v->setBits += 1;
    v->vector[i / 8] |= 1 << (i % 8);
  }
}
void bv_clr_bit(BitVector *v, uint32_t i) {
  //v->vector[i] = 0;
  v->vector[i / 8] &= ~(1 << (i % 8));
}
bool bv_get_bit(BitVector *v, uint32_t i) {
  //return v->vector[i];
  return ((v->vector[i / 8] & (1 << (i % 8))));
}
void bv_set_all_bits(BitVector *v) {
  for (uint32_t i = 0; i <= bv_get_len(v) * 8; i++) {
    bv_set_bit(v, i);
  }
}
