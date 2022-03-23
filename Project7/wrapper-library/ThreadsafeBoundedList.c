#include <stdio.h>
#include <stdlib.h>
#include "ThreadsafeBoundedList.h"

/* Contains functions to manipulate a thread-safe bounded doubly-linked list.  */

struct tsb_list {
    struct list *list;
    int capacity;
    int stop_requested;
    pthread_mutex_t mutex;
    pthread_cond_t listNotFull;
    pthread_cond_t listNotEmpty;
};

/* write all your functions below */

/* this function returns a pointer to the allocated tsb list */
struct tsb_list * tsb_createList(int (*equals)(const void *, const void *), char * (*toString)(const void *), void (*freeObject)(void *), int capacity){
	return NULL;
}

void tsb_freeList(struct tsb_list * list){
}

void tsb_addAtFront(struct tsb_list * list, NodePtr node){
}

void tsb_addAtRear(struct tsb_list * list, NodePtr node){
}

/* this function returns a pointer to the node that was removed */
NodePtr tsb_removeFront(struct tsb_list * list){
	return NULL;
}

/* this function returns a pointer to the node that was removed */
NodePtr tsb_removeRear(struct tsb_list * list){
	return NULL;
}

void tsb_finishUp(struct tsb_list * list){
}
