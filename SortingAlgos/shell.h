#ifndef __SHELL_H__
#define __SHELL_H__

#include "shell.h"

typedef struct Shell {
    int Compairison;
    int Swaps;
} Shell;

Shell *s_create();


//Sorts Arr using shellsort, input = array pointer
void shell_sort(int *Arr, Shell *s, int arr_len);


#endif

