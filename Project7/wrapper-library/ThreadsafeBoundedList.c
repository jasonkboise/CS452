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
    struct tsb_list *ret = (struct tsb_list*)malloc(sizeof(struct tsb_list));
    ret->list = createList(equals, toString, freeObject);
    ret->capacity = capacity;
    ret->stop_requested = 0;
    pthread_mutex_init(&(ret->mutex), NULL);
    pthread_cond_init(&(ret->listNotEmpty), NULL);
    pthread_cond_init(&(ret->listNotFull), NULL);
	return ret;
}

void tsb_freeList(struct tsb_list * list){
    freeList(list->list);
    pthread_mutex_destroy(&(list->mutex));
    pthread_cond_destroy(&(list->listNotEmpty));
    pthread_cond_destroy(&(list->listNotFull));
    free(list);
}

void tsb_addAtFront(struct tsb_list * list, NodePtr node){
    pthread_mutex_lock(&(list->mutex));
    while(list->list->size == list->capacity) {
        pthread_cond_wait(&(list->listNotFull), &(list->mutex));
    }
    addAtFront(list->list, node);
    if (list->list->size != list->capacity) {
        pthread_cond_signal(&(list->listNotFull));
    }
    if (list->list->size != 0) {
        pthread_cond_signal(&(list->listNotEmpty));
    }
    pthread_mutex_unlock(&(list->mutex));
}

void tsb_addAtRear(struct tsb_list * list, NodePtr node){
    pthread_mutex_lock(&(list->mutex));
    while(list->list->size == list->capacity) {
        pthread_cond_wait(&(list->listNotFull), &(list->mutex));
    }
    addAtRear(list->list, node);
    if (list->list->size != list->capacity) {
        pthread_cond_signal(&(list->listNotFull));
    }
    if (list->list->size != 0) {
        pthread_cond_signal(&(list->listNotEmpty));
    }
    pthread_mutex_unlock(&(list->mutex));
}

/* this function returns a pointer to the node that was removed */
NodePtr tsb_removeFront(struct tsb_list * list){
    pthread_mutex_lock(&(list->mutex));
    while(list->list->size == 0 && list->stop_requested == 0) {
        pthread_cond_wait(&(list->listNotEmpty), &(list->mutex));
    }
    NodePtr removed = removeFront(list->list);
    if (list->list->size != list->capacity) {
        pthread_cond_signal(&(list->listNotFull));
    }
    if (list->list->size != 0) {
        pthread_cond_signal(&(list->listNotEmpty));
    }
    pthread_mutex_unlock(&(list->mutex));
	return removed;
}

/* this function returns a pointer to the node that was removed */
NodePtr tsb_removeRear(struct tsb_list * list){
    pthread_mutex_lock(&(list->mutex));
    while(list->list->size == 0 && list->stop_requested == 0) {
        pthread_cond_wait(&(list->listNotEmpty), &(list->mutex));
    }
    NodePtr removed = removeRear(list->list);
    if (list->list->size != list->capacity) {
        pthread_cond_signal(&(list->listNotFull));
    }
    if (list->list->size != 0) {
        pthread_cond_signal(&(list->listNotEmpty));
    }
    pthread_mutex_unlock(&(list->mutex));
	return removed;
}

void tsb_finishUp(struct tsb_list * list){
    pthread_mutex_lock(&(list->mutex));
    list->stop_requested = 1;
    pthread_cond_broadcast(&(list->listNotEmpty));
    pthread_mutex_unlock(&(list->mutex));
}
