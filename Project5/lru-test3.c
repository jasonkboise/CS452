#include <stdio.h>
#include <stdlib.h>
#include "lru.h"

int main(int argc, char *argv[])
{
 LRUCache* obj = LRUCacheCreate(1);
 int value;
 value = LRUCacheGet(obj, 6);
 printf("what we get from the cache are [%d", value);
 value = LRUCacheGet(obj, 8);
 printf(" %d", value);
 LRUCachePut(obj, 12, 1);
 value = LRUCacheGet(obj, 2);
 printf(" %d", value);
 LRUCachePut(obj, 15, 11);
 LRUCachePut(obj, 5, 2);
 LRUCachePut(obj, 1, 15);
 LRUCachePut(obj, 4, 2);
 value = LRUCacheGet(obj, 5);
 printf(" %d]\n", value);
 LRUCachePut(obj, 15, 15);


 LRUCacheFree(obj);
}

/* vim: set ts=4: */
