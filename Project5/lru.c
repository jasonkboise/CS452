/* This program implements the LRUCache class which includes 4 functions.
 * 1. LRUCacheCreate(int capacity): Initialize the LRU cache with positive size capacity.
 * 2. int LRUCacheget(int key): Return the value of the key if the key exists, otherwise return -1.
 * 3. void LRUCacheput(int key, int value) Update the value of the key if the key exists. 
 * Otherwise, add the key-value pair to the cache. 
 * If the number of keys exceeds the capacity from this operation, evict the least recently used key.
 * 4. void LRUCacheFree(): Free the entire cache.
 */

#include <stdlib.h>
#include "lru.h"

LRUCache *cache;

LRUCache* LRUCacheCreate(int capacity) {
    cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->size = 0;
    cache->capacity = capacity;
    struct myListNode *hashList = malloc(sizeof(struct myListNode) * 10000);
    cache->head = NULL;
    cache->tail = NULL;
    cache->head = hashList;
    return cache;
}

int LRUCacheGet(LRUCache* obj, int key) {
    if (obj->hash[key] == NULL || obj->size == 0) {
        return -1;
    }
    else {
        //hash[key] is at front of list, needs to go to back
        if (obj->head == obj->hash[key]) {
            //move head pointer to next node
            obj->head = obj->hash[key]->next;
            //make new head's prev point to NULL
            obj->hash[key]->next->prev = NULL;
            //make the tail's next point to hash[key]
            obj->tail->next = obj->hash[key];
            //make hash[key]'s prev point to the current tail
            obj->hash[key]->prev = obj->tail;
            //make hash[key] the new tail
            obj->tail = obj->hash[key];
            //make hash[key]'s next point to NULL
            obj->hash[key]->next = NULL;

        }
        //hash[key] is already at the end of the list, don't need to move it
        else if (obj->tail == obj->hash[key]){

        }
        //hash[key] is in the middle of the list, move to the back
        else {
            //make the node before hash[key]'s next point to the one after hash[key]
            obj->hash[key]->prev->next = obj->hash[key]->next;
            //make the node after hash[key]'s prev point to the one before hash[key]
            obj->hash[key]->next->prev = obj->hash[key]->prev;
            //make the tail's next point to hash[key]
            obj->tail->next = obj->hash[key];
            //make hash[key]'s prev point to the tail
            obj->hash[key]->prev = obj->tail;
            //make hash[key]'s next point to NULL
            obj->hash[key]->next = NULL;
            //make hash[key] the new tail
            obj->tail = obj->hash[key];
        }
        //return the value of hash[key]
        return obj->hash[key]->value;
    }
    
}

void LRUCachePut(LRUCache* obj, int key, int value) {
}

void LRUCacheFree(LRUCache* obj) {
}

/* vim: set ts=4: */
