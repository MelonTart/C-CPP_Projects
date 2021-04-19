
#include "ll.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Constructor  for a ListNode.
//
// gs:  HatterSpeak struct containing oldspeak and its hatterspeak translation
//
ListNode *ll_node_create(HatterSpeak *gs) {
  ListNode *node = (ListNode *)malloc(sizeof(ListNode));
  node->gs = gs;
  node->next = NULL;
  node->len = 0;
  return node;
}

//
//  Destructor  for a ListNode.
//
// n:   The  ListNode  to free.
//
void ll_node_delete(ListNode *n) {
  free(n->gs);
  n->next = NULL;
  free(n);
}

//
//  Destructor  for a linked  list of  ListNodes.
//
// head:    The  head of the  linked  list.
//
void ll_delete(ListNode *head) {
  ListNode *checking = head;
  ListNode *ToDelete = head;
  while (ToDelete->next != NULL) {
    checking = checking->next; //goes to the next value
    ll_node_delete(ToDelete); //deletes the last one just used
    ToDelete = checking;
  }
  ll_node_delete(ToDelete); //deletes the last node in the list
}

//
//  Creates  and  inserts a ListNode  into a linked  list.
//
// head:   The  head of the  linked  list to  insert  in.
// gs:     HatterSpeak  struct.
//
ListNode *ll_insert(ListNode **head, HatterSpeak *gs, int index) {
  ListNode *node = ll_node_create(gs);
  node->len = 1;
  if (head[index] == NULL) {
    node->index = 1; //setting the index of the head
    head[index] = node; //first one at a given index
    return *head;
  }
  head[index]->len += 1;
  node->len += 1;
  ListNode *checking = head[index];

  while (checking->next != NULL) {
    checking = checking->next; //adding to the end of the linked list
    checking->len
        += 1; //adding to the length of every node as traversed as well as the created node
    node->len += 1;
  }
  node->index = node->len;
  checking->next = node;
  //printf("adding %s to the end LL\n",gs->oldspeak );
  return *head;
}

//
//  Searches  for a specific  key in a linked  list.
//  Returns  the  ListNode  if found  and  NULL  otherwise.
//
// head:    The  head of the  linked  list to  search  in.
// key:     The key to  search  for.
//
ListNode *ll_lookup(ListNode **head, char *key, int index, bool move_to_front) {
  //printf("searching for %s\n",key);

  int indexs_searched
      = 1; //used to count how many links have been checked to set index_last_found
  ListNode *checking
      = head[index]; //***possibly might need to look at the while loop
  ListNode *Previous; // the node before current

  if (checking == NULL) {
    return NULL; //it was not found as the hash table at this index is empty
  }

  if (strcmp(checking->gs->oldspeak, key) == 0) {
    checking->index_last_found = indexs_searched;
    return checking;
  }

  while (checking->next != NULL) {

    Previous = checking; //so that checking could be swapped if wanted
    checking = checking->next; //looking through the linked list for the word
    indexs_searched += 1;

    if (strcmp(checking->gs->oldspeak, key) == 0) {
      checking->index_last_found = indexs_searched;
      if (move_to_front) {
        Previous->next
            = checking->next; //next from previous changes to next from current
        checking->next = head[index];
        head[index] = checking;
      }
      return checking;
    }
  }

  if ((strcmp(checking->gs->oldspeak, key))) {
    return NULL;
  } else {
    checking->index_last_found = indexs_searched;
    if (move_to_front) {
      Previous->next
          = checking->next; //next from previous changes to next from current
      checking->next = head[index];
      head[index] = checking;
    }
    return checking;
  }
}
