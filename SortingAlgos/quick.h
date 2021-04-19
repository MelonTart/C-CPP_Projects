#ifndef __QUICK_H__
#define __QUICK_H__

#include "quick.h"

typedef struct Quick {
    int Compairison;
    int Swaps;
} Quick;

Quick *q_create();


//Sorts Arr using quicksort, input = array pointer
void quick_sort(int *Arr,int left,int right, Quick *q);

#endif

