#include "bubble.h"
#include <stdlib.h>
#include <stdio.h>

Bubble *bu_create() {
  struct Bubble *Bubble
      = (struct Bubble *)malloc(sizeof(struct Bubble));
  Bubble->Compairison = 0;
  Bubble->Swaps = 0;
  return Bubble;
}

void bubble_sort(int *Arr, Bubble *b, int arr_len){
    for(int i = 0; i< arr_len;i++){
        int j = arr_len - 1;
        while(j>i){
            b->Compairison +=1; //adding a compairison
            if(Arr[j] < Arr[j-1]){
                b->Swaps +=1; //adding a swap
                int temp = Arr[j];
                b->Swaps +=1;
                Arr[j] = Arr[j-1]; //swapping the elements
                b->Swaps +=1;
                Arr[j-1] = temp;
            }
            j -= 1; //incrementing j down as the last number is now correct
        }
    }
    return;
}

void counter(Bubble *b){
  printf("%d swaps\n",b->Swaps);
}

