#include "shell.h"
#include <stdlib.h>
#include <stdio.h>

Shell *s_create() {
  struct Shell *Shell
      = (struct Shell *)malloc(sizeof(struct Shell));
  Shell->Compairison = 0;
  Shell->Swaps = 0;
  return Shell;
}

int gap(int n){
  while(n>1){
      if(n<=2){
          return 1;
      }else{
          int a = n*5;
          a /= 11;
          return  a;
      }
  }
  return 1;
}

void shell_sort(int *Arr, Shell *s, int arr_len){
    int Gaps[arr_len];
    int next_gap = gap(arr_len);
    int num_gaps = 0;
    for(int x = 0;x<arr_len;x++){ //used to make an array of all the gaps
        num_gaps +=1;
        Gaps[x] = next_gap;
        if(Gaps[x] == 1){
          break; //dont need more than 1 gap of 1
        }
        next_gap = gap(next_gap);
    }

    for(int G =0;G<num_gaps;G++){ //used to loop thourgh the gaps
        int step = Gaps[G];
        //printf("%d Current Gap \n", step);
        for(int i = step;i < arr_len;i++){
            for(int y = i; y>=step-1 ;y-=step){
                //printf("am doing a compairison rn lol xd");
                s->Compairison +=1;
                if(Arr[y]<Arr[y-step]){
                    s->Swaps +=1;
                    s->Swaps +=1;
                    s->Swaps +=1;
                    int temp = Arr[y];
                    Arr[y] = Arr[y-step];
                    Arr[y-step] = temp;
                }
            }

        }
    }
    return;
}



