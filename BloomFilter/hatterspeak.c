#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "parser.h"
#include "speck.h"

int stdin_loadwords(char words[10000][20]) {
  for (int i = 0; i < 10000; i++) {
    for (int j = 0; j < 20; j++) {
      words[i][j] = 0; //used to clear the words arrays
    }
  }
  int index = 0;
  static int num_words = 0;

  char expression;
  char per = '.';
  char space = ' ';
  char endline = '\n';

  while ((scanf("%c", &expression))
         != EOF) //loads words untill it reachs EOF / end of file
  {
    //printf("%d is the current index \n",index);
    //printf("%d is the current word \n",num_words);
    if ((expression == per || expression == space || expression == endline)
        && index
               != 0) { //reset the index for the next word then increment the number of words
      index = 0;
      num_words += 1;
      continue;
    }
    if (!(expression == per || expression == space || expression == endline)) {
      words[num_words][index++] = expression;
    }
  }
  return num_words + 1; //adds the last word entered
}

void clearwords(char words[2][20]) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 20; j++) {
      words[i][j] = 0; //used to clear the word arrays
    }
  }
}
void str_to_lower(char str[20]) {
  int x = 0;
  char Letter;
  while (str[x]) { //while the character is non zero
    Letter = str[x];
    str[x] = tolower(str[x]); //used to switch input words into lowercase
    x += 1;
  }
}

void getword(char words[2][20], FILE *infile, regex_t *word_regex) {
  words[0][0] = *next_word(infile, word_regex);
  int space_yet = 0;
  int index = 0;
  char b = ' ';

  for (int x = 1; x < get_length(); x++) {
    if (space_yet == 0) {
      words[0][x] = *next_word(infile,
          word_regex); //used to read through the hatterspeak and oldspeak documents to load in the word
      if (words[0][x] == b) {
        space_yet = 1;
        index = x;
        words[0][x] = 0;
      }
    } else {
      words[1][x - index - 1] = *next_word(infile, word_regex);
    }
  }
  return;
}

int main(int argc, char **argv) {
  int statistics = 0;
  bool move_to_front_rule
      = 0; //setting the booleans to be used for stats or move to front rules

  int hash_table_size
      = 10000; //defaults for the sizes which can be changed with command line arguments below
  int bloom_filter_size = 10485760;

  for (int i = 0; i < argc; i++) { //loop thorugh command line arguments
    if (strcmp(argv[i], "-s") == 0) {
      statistics = 1;
    }
    if (strcmp(argv[i], "-m") == 0) {
      move_to_front_rule = 1;
    }
    if (strcmp(argv[i], "-b") == 0) {
      move_to_front_rule = 0;
    }
    if (strcmp(argv[i], "-h") == 0) {
      hash_table_size = atoi(argv[i + 1]); //sets the size of the hash table
    }
    if (strcmp(argv[i], "-f") == 0) {
      bloom_filter_size = atoi(argv[i + 1]); //sets the size of the bloom filter
    }
  }
  FILE *fp;
  FILE *fp2;
  fp = fopen("hatterspeak.txt", "r");
  fp2 = fopen("oldspeak.txt",
      "r"); //opens both the files, oldspeak and hatterspeak, to read words from
  char words[2][20]; //array used to store the words read in temporarily
  regex_t regex;
  regcomp(&regex, "[:word:a-zA-Z -]",
      0); //regex is used to find words with upper or lowwer case letters, and dashes.
  //The space allows us to get paired words at the same time

  HashTable *ht = ht_create(hash_table_size); //initalizing a hashtable

  clearwords(words); //clearing words arr

  for (
      int x = 0; x < 280;
      x++) { //Loop goes through all the words in hatterspeak.txt and creates a node in a linked list which is in a hashtable
    getword(words, fp, &regex);
    HatterSpeak *HS = (HatterSpeak *)malloc(sizeof(HatterSpeak));
    strcpy(HS->oldspeak,
        words
            [0]); //creates hatterspeak struct and sets the values of oldspeak and hatterspeak
    strcpy(HS->hatterspeak, words[1]);
    ht_insert(ht, HS);
    clearwords(
        words); //clears words so the array can be used again without error
  }
  fclose(fp);
  clear_words(); //clears the fuction

  BloomFilter *bloom
      = bf_create(bloom_filter_size); //initalizing the bloom filter

  //ListNode *gamer = ht_lookup(ht,"sleep");

  //for(int x = 0;x<100;x++){
  //    printf("%s", ht->heads[x]->next);
  //}

  for (int x = 0; x < 14281;
       x++) { //adds all the words from hatter speak to the bloom filter
    getword(words, fp2, &regex);
    HatterSpeak *HS = (HatterSpeak *)malloc(sizeof(HatterSpeak));
    strcpy(HS->oldspeak,
        words
            [0]); //creates hatterspeak struct and sets the values of oldspeak and hatterspeak
    strcpy(HS->hatterspeak, " ");
    ht_insert(ht, HS);
    bf_insert(bloom,
        words[0]); //since it is only single word lines it all goes to words[0]
    clearwords(words);
  }

  float num_link_lists = 0;
  float sum_of_links
      = 0; //used to get the number of linked lists and the sum_of_links which is the amount of nodes total used to find the avg link len

  for (int x = 0; x < hash_table_size; x++) {
    if (ht->heads[x] != NULL) {
      num_link_lists += 1;
      sum_of_links += ht->heads[x]->len;
    }
  }

  float avg_link_len = (sum_of_links / num_link_lists);

  int totalSetBits = 0; //bloom->filter->setBits;

  int len = (int)bv_get_len(bloom->filter); //used in the loop

  for (int x = 0; x < len;
       x++) { //loops to find the number of bits set within the bit vector
    if (bv_get_bit(bloom->filter, x)) {
      totalSetBits
          += 1; //This somehow generates over 10m errors in valgrind and I have 0 clue why
    }
  }

  //printf("%d\n",totalSetBits);
  //printf("%f\n",(float)(bloom->filter->length));
  float Bool_percentage
      = ((float)(totalSetBits) / (float)(bloom->filter->length)) * 100;
  float percent_of_hash = (num_link_lists / hash_table_size) * 100;

  //char *messange = "gamer moment";
  char *nontalk_mess = "Dear Wonderlander,\n\nYou have chosen to use words "
                       "that the queen has decreed oldspeak.\nDue to your "
                       "infraction you will be sent to the dungeon where you "
                       "will\n    be taught hatterspeak.\n\nYour errors:\n\n";
  char *hatter_mess = "Dear Wonderlander,\n\nThe decree for hatterspeak finds "
                      "your message lacking. Some of the\n    words that you "
                      "used are not hatterspeak.\nThe list shows how to turn "
                      "the old speak words into hatterspeak.\n\n";
  char *both_mess = "Dear Comrade,\n\nYou have chosen to use words that the "
                    "queen has decreed oldspeak.\nDue to your infraction you "
                    "will be sent to the dungeon where you will\n    be taught "
                    "hatterspeak.\n\nYour errors:\n\n";
  //messages typed out to be used when giving feedback on user input.

  char oldspeakwords
      [1000]
      [20]; //used to store old speak words so that we can throw the rebels in the dungon to rot
  int num_oldspeak = 0; //index used to keep track

  ListNode **hatterspeak = (ListNode **)calloc(1000,
      sizeof(ListNode
              *)); //creating a hatterspeak array to store the words and their potential translations from user input
  int num_hatterspeak = 0;

  float num_of_seeks = 0;
  float sum_of_seek_len = 0; //used to print statistics

  char stdin_words[10000][20]; //used to store the words the users inputs

  int num_words = stdin_loadwords(stdin_words);

  //printf("%d  number of words\n", num_words);
  // for(int x =0;x<5;x++){
  //     stdin_nextword(stdin_word);
  //     printf("%s \n",stdin_word);
  //     //clear_words();//clears the fuction
  // }

  //bool FalsePositive = 0;
  int probeOutput = 0;

  for (int x = 0; x < num_words; x++) { //loops until stdin runs out of words
    str_to_lower(stdin_words[x]); //changes the word to lowercase.
    //printf("%s \n",stdin_words[x]);
    probeOutput = 0;
    if (bf_probe(bloom,
            stdin_words[x])) { //if the word isnt found in the bloom filter
      //printf("%s did not pass the bloom filter \n",stdin_words[x]);
      probeOutput = 1;
      //strcpy(oldspeakwords[num_oldspeak++],stdin_words[x]);
    } //else check to see if it could be translated

    ListNode *temp = ht_lookup(ht, stdin_words[x],
        move_to_front_rule); //using the lookup fucntion to try and find it
    num_of_seeks += 1;
    sum_of_seek_len
        += 1; //was not specified if it counts as a seek len if there are 0 nodes, count as looking if there is nothing there??
    if (temp
        != NULL) { //if the word was found it is not NULL which means we save it as a node in hatterspeak
      if (strcmp(temp->gs->hatterspeak, " ")
          == 0) { //if its a oldspeak word in the hash table
        strcpy(oldspeakwords[num_oldspeak++],
            stdin_words[x]); //adding it to the list of the nono words they used
        sum_of_seek_len
            += temp->index_last_found; //adds the len of the LL as it must have traversed the whole thing
      } else {
        hatterspeak[num_hatterspeak++] = temp;
        sum_of_seek_len
            += temp->index_last_found
               - 1; //-1 because we already added 1 before when we looked it up
        //free(temp);
      }

    } else { //it could not be found and therefor is nontalk and is added to the oldspeak words array
      strcpy(oldspeakwords[num_oldspeak++],
          stdin_words[x]); //adding it to the list of the nono words they used
      int index
          = hash(ht->salt, stdin_words[x])
            % ht->length; //used to add to the sum_of_seek_len when no node was found
      if (ht->heads[index] != NULL) { //if the index exists
        sum_of_seek_len
            += ht->heads[index]
                   ->len; //adds the len of the LL as it must have traversed the whole thing
      }
    }
  }

  float avg_seek_len = sum_of_seek_len / num_of_seeks;

  if (statistics) { //if they wanted statistic in the command line arguments (typing -s )
    printf("Seeks: %f\n", num_of_seeks);
    printf("Average seek length: %f\n", avg_seek_len);
    printf("Average Linked List Length: %f\n", avg_link_len);
    printf("Hash table load: %f\n", percent_of_hash);
    printf("Bloom filter load: %f\n", Bool_percentage);
  } else {
    if (num_oldspeak > 0
        && num_hatterspeak
               > 0) { //if there are both words to be translated and nontalk/banned words
      printf("%s", both_mess);
      for (int x = 0; x < num_oldspeak; x++) {
        printf("%s\n", oldspeakwords[x]);
      }
      printf("\n");
      printf("Appropriate hatterspeak translations.\n\n");
      for (int x = 0; x < num_hatterspeak; x++) {
        printf("%s -> %s\n", hatterspeak[x]->gs->oldspeak,
            hatterspeak[x]->gs->hatterspeak);
      }

    } else if (num_oldspeak
               > 0) //if they said banned words and no words to be translated
    {
      printf("%s", nontalk_mess);
      for (int x = 0; x < num_oldspeak; x++) {
        printf("%s\n", oldspeakwords[x]);
      }
    } else if (num_hatterspeak
               > 0) //if they only said words that need to be translated
    {
      printf("%s", hatter_mess);
      for (int x = 0; x < num_hatterspeak; x++) {
        printf("%s -> %s\n", hatterspeak[x]->gs->oldspeak,
            hatterspeak[x]->gs->hatterspeak);
      }
    }
  }

  clear_words();
  free(hatterspeak);
  fclose(fp2);
  regfree(&regex); //freeing up all the memory we used to make this work :))))
  bf_delete(bloom);
  ht_delete(ht);
  return 0;
}
