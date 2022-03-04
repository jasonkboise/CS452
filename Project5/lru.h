struct myListNode {
    int key;
    int value;
    struct myListNode *next;
    struct myListNode *prev;
};

typedef struct {
    int capacity;
    int size;
    struct myListNode *head;
    struct myListNode *tail;
    struct myListNode *hash[10000];
} LRUCache;

LRUCache* LRUCacheCreate(int capacity);

int LRUCacheGet(LRUCache* obj, int key);

void LRUCachePut(LRUCache* obj, int key, int value);

void LRUCacheFree(LRUCache* obj);
