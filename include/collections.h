#ifndef _SVEN_COLLECTIONS_H
#define _SVEN_COLLECTIONS_H

#include <stdlib.h>
#include <stdbool.h>

//--------------------------------------------------//
//                                                  //
//                       LIST                       //
//                                                  //
//--------------------------------------------------//
typedef struct _SvenList SvenList;

// creates a SvenList
SvenList *SvenList_create();

// destroys a SvenList
void SvenList_destroy(SvenList *list, bool deep);

// calls free on all data from SvenList and destroys all nodes
void SvenList_clear(SvenList *list);

// calls destroy on all data from SvenList and destroys all nodes
void SvenList_clearWith(SvenList *list, void(*destroy)(void *) );

//adding
void SvenList_push(SvenList *list, void *elem);
void SvenList_append(SvenList *list, void *elem);
void SvenList_add(SvenList *list, void *elem, int i);

//getting simple
void *SvenList_front(SvenList *list);
void *SvenList_back(SvenList *list);
void *SvenList_get(SvenList *list, int i);

//getting continued
void *SvenList_pop2(SvenList *list); //pop front and return
void *SvenList_remove2(SvenList *list, int i); //pop at i and return

//applies function to all nodes of SvenList
void SvenList_map(SvenList *list, void(*function)(void *) );

//--------------------------------------------------//
//                                                  //
//                       HASH                       //
//                                                  //
//--------------------------------------------------//

#endif
