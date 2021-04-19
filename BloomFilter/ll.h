
#ifndef __LL_H__
#define __LL_H__

#ifndef NIL
#define NIL (void *)0
#endif

#include <stdbool.h>

// If flag is set , ListNodes  that  are  queried  are  moved to the  front.
extern bool move_to_front;

typedef struct ListNode ListNode;

typedef struct HatterSpeak {
  char oldspeak[20];
  char hatterspeak[20];
} HatterSpeak;

//
//  Struct  definition  of a ListNode.
//
// gs: HatterSpeak struct containing oldspeak and its hatterspeak translation.
//
struct ListNode {
  int len; //used to find avg len of LL
  int index; //used to find avg len searches into a LL starting at 1
  int index_last_found; //used to avoid issues when switching
  HatterSpeak *gs;
  ListNode *next;
};

//  Constructor  for a ListNode.
//
// gs:  HatterSpeak struct containing oldspeak and its hatterspeak translation
//
ListNode *ll_node_create(HatterSpeak *gs);

//  Destructor  for a ListNode.
//
// n:   The  ListNode  to free.
//
void ll_node_delete(ListNode *n);

//
//  Destructor  for a linked  list of  ListNodes.
//
// head:    The  head of the  linked  list.
//
void ll_delete(ListNode *head);

//
//  Creates  and  inserts a ListNode  into a linked  list.
//
// head:   The  head of the  linked  list to  insert  in.
// gs:     HatterSpeak  struct.
//
ListNode *ll_insert(ListNode **head, HatterSpeak *gs, int index);

//
//  Searches  for a specific  key in a linked  list.
//  Returns  the  ListNode  if found  and  NULL  otherwise.
//
// head:    The  head of the  linked  list to  search  in.
// key:     The key to  search  for.
//
ListNode *ll_lookup(ListNode **head, char *key, int index, bool move_to_front);

#endif
