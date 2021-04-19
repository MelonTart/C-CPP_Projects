#include "binary.h"
#include <stdlib.h>

Binary *b_create() {
  struct Binary *Binary
      = (struct Binary *)malloc(sizeof(struct Binary));
  Binary->Swaps = 0;
  Binary->Compairison = 0;
  return Binary;
}

void binary_sort(int *Arr, Binary *b, int arr_size){
    for(int i = 1;i<arr_size;i++){
      int value = Arr[i];
      int left = 0;
      int right = i;

      while(left<right){
        b->Compairison+=1; //comparing value to Arr[mid]
        int mid = left + ((right - left) / 2);
        if(value >= Arr[mid]){
          left = mid + 1;
        }else{
          right = mid;
        }
      }
      for(int j=i;j>left;j-=1){
          b->Swaps+=1;
          b->Swaps+=1;
          b->Swaps+=1;
          int temp = Arr[j];
          Arr[j] = Arr[j-1]; //swaps them
          Arr[j-1] = temp;
      }
    }
    return;
}


