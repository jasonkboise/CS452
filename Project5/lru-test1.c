#include <stdio.h>
#include <stdlib.h>
#include "lru.h"

int main(int argc, char *argv[])
{
 LRUCache* obj = LRUCacheCreate(1);
 int value;
 LRUCachePut(obj, 2, 1);
 value=LRUCacheGet(obj, 2);
 printf("what we get from the cache are [%d", value);
 LRUCachePut(obj, 3, 2);
 value = LRUCacheGet(obj, 2);
 printf(" %d", value);
 value = LRUCacheGet(obj, 3);
 printf(" %d]\n", value);

 LRUCacheFree(obj);
 return 0;
}

/* vim: set ts=4: */
