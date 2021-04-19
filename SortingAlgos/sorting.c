#include "bubble.h"
#include "binary.h"
#include "shell.h"
#include "quick.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int RAND_MAXs =  1073741823;


int random(int max){
   return rand() / (RAND_MAX / (max + 1) + 1);
}

//making a fixsssss

int main(int argc, char **argv)
{
    int bubble = 0;
    int binary = 0;
    int quick = 0;
    int shell = 0;
    int print_num = 100;
    int rand_seed = 8222022;
    int arr_size = 100;

    
    for (int i = 0; i < argc; i++) { //loop thorugh command line arguments
        if (strcmp(argv[i], "-b") == 0) {
            bubble = 1;
        }   
        if (strcmp(argv[i], "-s") == 0) {
            shell = 1;
        }
        if (strcmp(argv[i], "-q") == 0) {
            quick = 1;
        }
        if (strcmp(argv[i], "-i") == 0) {
            binary = 1;
        }
        if (strcmp(argv[i], "-p") == 0) {
            print_num = atoi(argv[i + 1]); //sets the number of elements to print
        }
        if (strcmp(argv[i], "-n") == 0) {
            arr_size = atoi(argv[i + 1]);//sets the size of the array
        }
        if (strcmp(argv[i], "-r") == 0) {
            rand_seed = atoi(argv[i + 1]); // sets the seed for the random number generator
        }
        if (strcmp(argv[i], "-A") == 0) {
            bubble = 1; //will do all the prints with the argument -A
            quick = 1;
            binary = 1;
            shell = 1;
        }
  }


  int *q_array = (int*)calloc(arr_size, sizeof(long)); //make the dynamically allocated arrays for each algorithem to sort
  int *bubble_array = (int*)calloc(arr_size, sizeof(long));
  int *binary_array = (int*)calloc(arr_size, sizeof(long));
  int *s_array = (int*)calloc(arr_size, sizeof(long));
  srand(rand_seed); //setting the random seed


  for(int x =0; x<arr_size;x++){
      int rand = random(RAND_MAXs);  //adding the random numbers into the arrays
      q_array[x] = rand;
      bubble_array[x] = rand;
      binary_array[x] = rand;
      s_array[x] = rand;
  }

  if(binary){
      printf("Binary Insertion Sort\n");
      Binary *binary_sorter = b_create(); //creates sorter object
      binary_sort(binary_array, binary_sorter,arr_size);//sorting the arrays 
      printf("%d elements, %d moves, %d compares\n",arr_size,binary_sorter->Swaps,binary_sorter->Compairison);//printing header
      for(int x = 0;x<print_num;x+=1){
          printf("%*d  ",9,binary_array[x]);//printing array
          if( ((x+1)%7) == 0){
              printf("\r\n");
          }
      }
      printf("\n");
      
      free(binary_sorter);
  }
  if(quick){
      printf("Quick Sort\n");
      Quick *quick_sorter =  q_create();//creates sorter object
      quick_sort(q_array,0,arr_size-1,quick_sorter);//sorting the arrays 

    printf("%d elements, %d moves, %d compares\n",arr_size,quick_sorter->Swaps,quick_sorter->Compairison);//printing header
      for(int x = 0;x<print_num;x+=1){
          printf("%*d  ",9,q_array[x]);//printing array
          if( ((x+1) %7) == 0 ){
              printf("\r\n");
          }
      }
      printf("\n");

      free(quick_sorter);
  }
if(shell){
      printf("Shell Sort\n");
      Shell *shell_sorter = s_create();//creates sorter object
      shell_sort(s_array, shell_sorter,arr_size);//sorting the arrays 

    printf("%d elements, %d moves, %d compares\n",arr_size,shell_sorter->Swaps,shell_sorter->Compairison);//printing header
      for(int x = 0;x<print_num;x+=1){
          printf("%*d  ",9,s_array[x]); //printing array
          if( ((x+1) %7) == 0){
              printf("\r\n");
          }
      }
      printf("\n");

      free(shell_sorter);
  }



 if(bubble){
    printf("Bubble Sort\n");
    Bubble *bubble_sorter = bu_create(); //initallizing the sorters
    bubble_sort(bubble_array, bubble_sorter,arr_size); //sorting the arrays 

    printf("%d elements, %d moves, %d compares\n",arr_size,bubble_sorter->Swaps,bubble_sorter->Compairison);//printing header
      for(int x = 0;x<print_num;x+=1){
          printf("%*d  ",9,bubble_array[x]);//printing array
          if( ((x+1) %7) == 0){
              printf("\r\n");
          }
      }
      printf("\n");


    free(bubble_sorter);
  }



    free(q_array);
    free(s_array);
    free(bubble_array);
    free(binary_array);



    /* code */
    return 0;
}
