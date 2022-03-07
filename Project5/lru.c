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
    //struct myListNode *hashList = malloc(sizeof(struct myListNode) * 10000);
    cache->head = NULL;
    cache->tail = NULL;
    int i = 0;
    while (i < 10000) {
        cache->hash[i] = NULL;
        i++;
    }
    return cache;
}

int LRUCacheGet(LRUCache* obj, int key) {
    if (obj->hash[key] == NULL || obj->size == 0) {
        return -1;
    }
    else {
        //hash[key] is the only thing in the list, no need to move
        if (obj->head == obj->tail) {

        }
        //hash[key] is at front of list, needs to go to back
        else if (obj->head == obj->hash[key]) {
            struct myListNode *p1;
            p1 = obj->head->next;
            //make node after head's prev point to NULL
            p1->prev = NULL;
            //move head pointer to next node
            obj->head = p1;
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
            struct myListNode *p1, *p2;
            p1 = obj->hash[key]->prev;
            p2 = obj->hash[key]->next;
            //make the node before hash[key]'s next point to the one after hash[key]
            p1->next = p2;
            //make the node after hash[key]'s prev point to the one before hash[key]
            p2->prev = p1;
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
    //check if the key is already in the hash
    int check = LRUCacheGet(obj, key);
    if (check != -1) {
        //hash[key] is already in the list. Was moved to the back.
        //now you just need to update the value.
        obj->hash[key]->value = value;
    }
    else {
        //not found, so you need to make a new node and
        //add it to the hash, also increment size

        struct myListNode *node = (struct myListNode*)malloc(sizeof(struct myListNode));
        node->key = key;
        node->value = value;
        node->next = NULL;
        node->prev = NULL;

        if (obj->size == obj->capacity) {
            struct myListNode *remove = obj->head;
            if (obj->head == obj->tail) {
                obj->head = node;
                obj->tail = node;
                obj->hash[remove->key] = NULL;
                free(remove);
            }
            else {
                //remove the head
                obj->head->next->prev = NULL;
                obj->head = obj->head->next;
                obj->hash[remove->key] = NULL;
                free(remove);        
                
                //add node to tail
                obj->tail->next = node;
                node->prev = obj->tail;
                obj->tail = node;
            }
        }
        else {
            obj->size = obj->size + 1;
            if (obj->tail == NULL) {
                obj->tail = node;
                obj->head = node;
                obj->tail->next = NULL;
                obj->tail->prev = NULL;
            }
            else {
                obj->tail->next = node;
                node->prev = obj->tail;
                obj->tail = node;
            }
            
        }
        obj->hash[key] = node;
        
    }
}

void LRUCacheFree(LRUCache* obj) {
    struct myListNode *curr = obj->head;
    if (curr != NULL) {
        while(curr->next != NULL) {
            struct myListNode *next = curr->next;
            free(curr);
            curr = next;
        }
        free(curr);
    }
    free(obj);
}

/* vim: set ts=4: */
