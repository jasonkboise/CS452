#include <stdio.h>
#include <stdlib.h>
#include "lru.h"

int main(int argc, char *argv[])
{
 LRUCache* obj = LRUCacheCreate(2);
 LRUCachePut(obj, 1, 1);
 LRUCachePut(obj, 2, 2);
 int value;
 value=LRUCacheGet(obj, 1);
 printf("what we get from the cache are [%d", value);
 LRUCachePut(obj, 3, 3);
 value = LRUCacheGet(obj, 2);
 printf(" %d", value);
 LRUCachePut(obj, 4, 4);
 value=LRUCacheGet(obj, 1);
 printf(" %d", value);
 value=LRUCacheGet(obj, 3);
 printf(" %d", value);
 value=LRUCacheGet(obj, 4);
 printf(" %d]\n", value);

 LRUCacheFree(obj);
 return 0;
}

/* vim: set ts=4: */
