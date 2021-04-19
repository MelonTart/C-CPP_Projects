#include "bv.h"
#include "sieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void removeSpaces(char *
        str) //Fuction from https://www.geeksforgeeks.org/remove-spaces-from-a-given-string/ to remove spaces
{
  // To keep track of non-space character count
  int count = 0;

  // Traverse the given string. If current character
  // is not space, then place it at index 'count++'
  for (int i = 0; str[i]; i++)
    if (str[i] != ' ')
      str[count++] = str[i]; // here count is
          // incremented
  str[count] = '\0';
}

//Description of check functions within design.pdf
int FibCheck(int number) {
  int num1 = 1;
  int num2 = 1;
  while (num1 <= number) {
    if (num1 == number) {
      return 1;
    }
    //printf("%d\n",num1);
    int Temp = num2;
    num2 = num2 + num1;
    num1 = Temp;
  }
  return 0;
}
int LucasCheck(int number) {
  int num1 = 2;
  int num2 = 1;
  while (num1 <= number) {
    if (num1 == number) {
      return 1;
    }
    int Temp = num2;
    num2 = num2 + num1;
    num1 = Temp;
  }
  return 0;
}
int MersCheck(int number) {
  int Power = 2;
  while (Power - 1 <= number) {
    if (Power - 1 == number) {
      return 1;
    }
    Power = Power * 2;
  }
  return 0;
}
//PalindromeCheck better described in Design pdf
int PalCheck(int number, int base) {
  int array[1000000]; //builds an array of all the digits in order
  int Top = 0;
  while (number > 0) {
    int r = number % base;
    //printf("%d \n",r);
    array[Top] = r; //converting it to the given base with the remainder
    Top += 1;
    number = (number / base);
  }

  if (Top == 1) {
    //printf("only 1 number\n");
    return 1; //edge case for single
  }
  int Bot = 0;
  Top -= 1;
  //printf("%d %d \n",Top,Bot);
  while (1) {
    //checks the current top and bottom then moves both in towards each other
    if (array[Top] == array[Bot]) {
      if ((Bot + 1 == Top) || (Bot + 1 == Top - 1)) {
        //end case of pal where either they meet in the middle or cross
        return 1;
      }
      Top -= 1;
      Bot += 1;
    } else {
      return 0; //if they dont match at one point they ane not palindromes
    }
  }
}

void convert(int decimal, int base_to, char *converted_to_base_array) {
  int m = 0;
  char ArrChar[23]
      = "0123456789abcdefghijklm"; //used to convert numbers to base 23 for my name
  int index = 0;

  while (decimal != 0) { //continues untill converted
    m = m * 10;
    int d = decimal / base_to;
    int r = decimal
            % base_to; //remainder is the number used for current index digit
    converted_to_base_array[index]
        = ArrChar[r]; //converts number to char representation
    //printf("%c \n",converted_to_base_array[index]);
    index++;
    m += r;
    decimal = d;
  }
  return;
}

void PrintP(BitVector *v, int base, int largest_number) {
  printf("Base %d\n", base);
  printf("---- --\n");
  for (int x = 0; x <= largest_number; x++) {
    if (bv_get_bit(v, x)) {
      //printf("%d\n",x);
      if (PalCheck(x, base)) {
        char converted_to_base_array[1000];
        for (int x = 0; x < 1000; x++) {
          converted_to_base_array[x] = 0;
        }
        convert(x, base, converted_to_base_array); //converts array
        removeSpaces(converted_to_base_array); //removes spaces
        printf("%d = %s\n", x, converted_to_base_array);
      }
    }
  }
  printf("\n");
  return;
}

int main(int argc, char **argv) {
  int print_prime = 0; //booleans to print or not
  int print_pal = 0;
  int largest_number = 1000;

  for (int i = 0; i < argc; i++) { //loop thorugh command line arguments
    if (strcmp(argv[i], "-s") == 0) {
      print_prime = 1;
    }
    if (strcmp(argv[i], "-p") == 0) {
      print_pal = 1;
    }
    if (strcmp(argv[i], "-n") == 0) {
      largest_number = atoi(argv[i + 1]); //gets the number following n
    }
  }
  BitVector *BitVec = bv_create(largest_number);
  sieve(BitVec); //finding the prime numbers using sieve
  if (print_prime) {
    for (int x = 0; x <= largest_number; x++) {
      if (bv_get_bit(BitVec, x)) {
        printf("%d: prime", x);
        if (MersCheck(x)) {
          printf(", mersenne");
        }
        if (LucasCheck(x)) {
          printf(", lucas");
        }
        if (FibCheck(x)) {
          printf(", fibonacci");
        }
        printf("\n");
      }
    }
    printf("\n");
  }
  if (print_pal) {
    PrintP(BitVec, 2, largest_number);
    PrintP(BitVec, 9, largest_number);
    PrintP(BitVec, 10, largest_number);
    PrintP(BitVec, 23, largest_number);
  }
  bv_delete(BitVec);

  return 0;
}
