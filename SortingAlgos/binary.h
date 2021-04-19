#ifndef __BINARY_H__
#define __BINARY_H__

#include "binary.h"

typedef struct Binary {
    int Compairison;
    int Swaps;
} Binary;

Binary *b_create();


//Sorts Arr using binarysort, input = array pointer
void binary_sort(int *Arr , Binary *b,int arr_size) ;

#endif

