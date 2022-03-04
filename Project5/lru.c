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
}

void LRUCachePut(LRUCache* obj, int key, int value) {
}

void LRUCacheFree(LRUCache* obj) {
}

/* vim: set ts=4: */
