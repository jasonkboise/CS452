#include <stdio.h>
#include <stdlib.h>
#include "lru.h"

int main(int argc, char *argv[])
{
 LRUCache* obj = LRUCacheCreate(2);
 int value;
 LRUCachePut(obj, 2, 1);
 LRUCachePut(obj, 2, 2);
 value = LRUCacheGet(obj, 2);
 printf("what we get from the cache are [%d", value);
 LRUCachePut(obj, 1, 1);
 LRUCachePut(obj, 4, 1);
 value=LRUCacheGet(obj, 2);
 printf(" %d]\n", value);

 LRUCacheFree(obj);
 return 0;
}

/* vim: set ts=4: */
