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
  BitVector->vector = (uint8_t *)malloc(BitVector->length * sizeof(uint8_t));
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
  v->vector[i / 8] |= 1 << (i % 8);
}
void bv_clr_bit(BitVector *v, uint32_t i) {
  //v->vector[i] = 0;
  v->vector[i / 8] &= ~(1 << (i % 8));
}
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
  //return v->vector[i];
  return ((v->vector[i / 8] & (1 << (i % 8))));
}
void bv_set_all_bits(BitVector *v) {
  for (uint32_t i = 0; i <= bv_get_len(v) * sizeof(uint8_t); i++) {
    bv_set_bit(v, i);
  }
}
