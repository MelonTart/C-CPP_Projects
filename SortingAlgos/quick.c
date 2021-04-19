
#include "quick.h"
#include <stdlib.h>


Quick *q_create() {
  struct Quick *Quick
      = (struct Quick *)malloc(sizeof(struct Quick));
  Quick->Swaps = 0;
  Quick->Compairison = 0;
  return Quick;
}
int Partition(int *Arr,int left,int right,Quick *q){
  int pivot = Arr[left];
  int lo = left +1;
  int hi = right;



  while(1){
    while( ((lo <= hi) && (Arr[hi] >= pivot)) ){
      q->Compairison+=1;
      hi-=1;
    }
    while( ((lo <= hi) && (Arr[lo] <= pivot)) ){
      q->Compairison+=1;
      lo+=1;
    }
    q->Compairison+=1;
    if(lo<=hi){
      q->Swaps+=1;
      q->Swaps +=1;
      q->Swaps +=1;
      int temp = Arr[lo];
      Arr[lo] = Arr[hi];
      Arr[hi] = temp;
    }else{
      break;
    }
  }

  q->Swaps+=1;
  q->Swaps +=1;
  q->Swaps +=1;
  int temp = Arr[left];
  Arr[left] = Arr[hi];
  Arr[hi] = temp;
  return hi;
}





void quick_sort(int *Arr,int left,int right, Quick *q){
    if(left<right){
      int index = Partition(Arr,left,right,q);
      quick_sort(Arr,left,index-1,q);
      quick_sort(Arr,index+1,right,q);
    }
    return;
}


