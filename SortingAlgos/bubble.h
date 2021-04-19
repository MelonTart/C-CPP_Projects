#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "bubble.h"

typedef struct Bubble {
    int Compairison;
    int Swaps;
} Bubble;

Bubble *bu_create();


//Sorts Arr using bubblesort, input = array pointer
void bubble_sort(int *Arr, Bubble *b, int arr_len);

void counter(Bubble *b);
#endif

