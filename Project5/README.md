## Overview

In this assignment we will implement a least recently used (LRU) policy, a commonly used page replacement policy in OS to manage physical memory (i.e., swapping pages in and out of the physical memory), as well as manage cache (e.g., the TLB cache). Your algorithm must be in an amortized O(1) time complexity. Note this is NOT a kernel project, and you should just develop your code on onyx, not in your virtual machine. Submissions fail to compile or run on onyx, will not be graded.

## Learning Objectives

- Understand the least recently used (LRU) policy.

## Note

This README is all about implementation. The theory of how LRU works is beyond the scope of this README. 

In this assignment, we use a doubly linked list, which has one or many nodes, to represent the cache, which has one or many entries. Thus in this README, the term entry and node are equivalent, it is just we use the term **entry** when we talk about the cache, but use the term **node** when we talk about the list.

## References

LRU is used in more than one situations, and thus is described in multiple chapters.

Operating Systems: Three Easy Pieces: [Chapter 19: Translation Lookaside Buffers](https://pages.cs.wisc.edu/~remzi/OSTEP/vm-tlbs.pdf)<br/>
Operating Systems: Three Easy Pieces: [Chapter 22: Swapping: Policies](https://pages.cs.wisc.edu/~remzi/OSTEP/vm-beyondphys-policy.pdf)<br/>

This assignment is adapted from Leetcode problem 146. It is not clear which company or who created this interview problem, although the LRU algorithm itself has been around for a long time.

## Starter Code

The starter code looks like this:

```console
(base) [jidongxiao@onyx cs452-swapping]$ ls
lru.c  lru.h  lru-test1.c  lru-test2.c  lru-test3.c  lru-test4.c  lru-test5.c  lru-test6.c  lru-test7.c  Makefile  README.md  runval.sh
```

You will be completing the lru.c file. You should not modify the lru.h file.

## Specification

You are required to implement the following 4 functions.

```c
LRUCache* LRUCacheCreate(int capacity);
```

Users call this function to create a cache whose size is *capacity*, i.e., the cache can have at most *capacity* entries. *capacity* is an integer equal to or greater than 1.

```c
void LRUCachePut(LRUCache* obj, int key, int value);
```

Users call this function to add an entry to the cache. An entry is represented by a *key-value* pair. If the key exists, then update its corresponding value. If the number of keys exceeds the capacity of cache, then evict the least recently used (LRU) entry first, before you add the new entry. This README file also refers to this function as the *put*() function.

Note: if you want to have a better understanding of what a real entry looks like in a real cache, you can read the book chapter 19.4, 19.5, 19.6 and 19.7. In the chapter's context, **VPN** is the *key*, and **PFN** is the *value*.

```c
int LRUCacheGet(LRUCache* obj, int key);
```

Users call this function to get the entry which matches with the *key*. If such an entry does not exist, return -1. If such an entry does exist, return its value. This README file also refers to this function as the *get*() function.

```c
void LRUCacheFree(LRUCache* obj);
```

Users eventually will call this function to release the memory for: 1. all the entries; 2. the cache itself. This README file also refers to this function as the *cachefree*() function.

## Global Data Structures and Variables

The starter code defines the following global data structures and variables, in lru.h. Once again, do not modify lru.h.

```c
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
```

We define *LRUCache* to represent the entire cache we need to manage. To achieve the goal of amortized O(1) time complexity, we utilize two key data structures: 

- we use a doubly linked list to maintain all cache entries - each entry is represented by an instance of *struct myListNode*. A doubly linked list allows us to quickly remove a node from the front and add a node to the back, because we have a pointer *head* which points to the beginning of the list, and we keep a pointer *tail* which points to the rear end of the list. We consider the node at the beginning of the list as the least recently used entry. Any time we add a new entry or update an existing entry, we put it or move it to the back of the list. Any time we access an existing entry via *get*(), we also move such an entry to the back of the list.

- we use a hash table to accelerate the searching as well as the updating process. In *get*(), we need to search the list so as to find an entry which matches with the *key*, which is the parameter of the *get*() function. In *put*(), we may need to update an entry. Without a hash table, searching a doubly linked list would be an O(n) operation, and updating a random entry in the doubly linked list would also be an O(n) operation. To make it an O(1) operation, we leverage a hash table, which is why the struct *LRUCache* has a field called *hash*. In this assignment, we assume all the keys will be in between 0 to 9999. Thus the *hash* array has 10000 entries. We use *hash[0]* to track the address of the entry (i.e., the node) whose key is 0, and use *hash[1]* to track the address of the entry (i.e., the node) whose key is 1, and use *hash[2]* to track the address of the entry (i.e., the node) whose key is 2, ..., and use *hash[9999]* to track the address of the entry (i.e., the node) whose key is 9999. Given that this hash table is essentially an array which tracks the address of all the entries of the cache, updating or searching an entry in the cache therefore becomes an O(1) operation.

- **size**. The *size* field in the LRUCache struct tells us the current size of the cache, i.e., how many entries are existing in the cache. At first, *size* is 0. Any time you call *put*() to add an entry to the cache, you increment *size* by 1. If the *put*() function just updates the vaule of an existing entry, then you should NOT increment size. When eviction is happening, you may or may not want to decrement *size* by 1 - depending on how you implement your *put*() function. Your *get*() function should not change the *size* of the cache, as *get*() is just reading information from the cache.

We define the struct *myListNode* to represent each node in the doubly linked list, once again, each node represents one entry in the cache.

## APIs and Helper Code

The only API functions you need to call in this assignment is *malloc*() and *free*(). And that is why \<stdlib.h\> is already included in lru.c.

There is no helper code provided in this assignment. You probably do not need any.

## Testing

7 testing programs are provided. They are lru-test[1-7].c. Once you run *make*, you will generate the binary files of these testing programs. lru-test[1-6] tests the correctness of your algorithm, whereas test7 tests the time complexity of your algorithm.

## Expected Results

When running lru-test[1-6], you should get the exact same results as following:

```console
(base) [jidongxiao@onyx lru]$ ./lru-test1 
what we get from the cache are [1 -1 2]
(base) [jidongxiao@onyx lru]$ ./lru-test2
what we get from the cache are [1 -1 -1 3 4]
(base) [jidongxiao@onyx lru]$ ./lru-test3
what we get from the cache are [-1 -1 -1 -1]
(base) [jidongxiao@onyx lru]$ ./lru-test4
what we get from the cache are [2 -1]
(base) [jidongxiao@onyx lru]$ ./lru-test5
what we get from the cache are [-1 3]
(base) [jidongxiao@onyx lru]$ ./lru-test6
what we get from the cache are [4 3 2 -1 -1 2 3 -1 5]
```

If you do not get the exact same results, your implementation is wrong.

When running lru-test7, which tests the time complexity of your algorithm, you are expected to get results similar to this:

```console
(base) [jidongxiao@onyx lru]$ ./lru-test7
cache capacity 10000, 1000000 put operations (mostly existing key) took 16.40 milliseconds.
cache capacity 10000, 1000000 get operations (mostly key exists) took 11.67 milliseconds.
cache capacity 8000, 1000000 put operations (mostly existing key) took 16.39 milliseconds.
cache capacity 8000, 1000000 get operations (mostly key exists) took 11.22 milliseconds.
cache capacity 4000, 1000000 put operations (mostly existing key) took 16.40 milliseconds.
cache capacity 4000, 1000000 get operations (mostly key exists) took 11.24 milliseconds.
cache capacity 3000, 1000000 put operations (mostly existing key) took 16.90 milliseconds.
cache capacity 3000, 1000000 get operations (mostly key exists) took 11.66 milliseconds.
cache capacity 2000, 1000000 put operations (mostly existing key) took 16.46 milliseconds.
cache capacity 2000, 1000000 get operations (mostly key exists) took 11.24 milliseconds.
cache capacity 1000, 1000000 put operations (mostly existing key) took 16.39 milliseconds.
cache capacity 1000, 1000000 get operations (mostly key exists) took 11.25 milliseconds.

cache capacity 100, 1000000 put operations (mostly new key) took 31.07 milliseconds.
cache capacity 100, 1000000 get operations (mostly key not found) took 5.07 milliseconds.
cache capacity 10, 1000000 put operations (mostly new key) took 31.18 milliseconds.
cache capacity 10, 1000000 get operations (mostly key not found) took 5.05 milliseconds.
cache capacity 3, 1000000 put operations (mostly new key) took 31.27 milliseconds.
cache capacity 3, 1000000 get operations (mostly key not found) took 5.06 milliseconds.
cache capacity 2, 1000000 put operations (mostly new key) took 31.11 milliseconds.
cache capacity 2, 1000000 get operations (mostly key not found) took 5.06 milliseconds.
cache capacity 1, 1000000 put operations (mostly new key) took 11.11 milliseconds.
cache capacity 1, 1000000 get operations (mostly key not found) took 5.08 milliseconds.
```

- For cache capacity 10000, 8000, 4000, 3000, 2000, 1000, calling your *put*() function (1,000,000 times) should take approximately the same amount of time.
- For cache capacity 100, 10, 3, 2, calling your *put*() function (1,000,000 times) should take approximately the same amount of time. These cases take more time than the above cases, because they mainly deal with new key situations, which may require evicting entries; whereas the above cases mainly deal with existing key situations, which do not require evicting entries.
- For cache capacity 1, calling your *put*() function (1,000,000 times) takes less time than the above cases, but cache capacity 1 is a unique situation, because it does not involve any linked list operations .

- For cache capacity 10000, 4000, 3000, 2000, 1000, calling your *get*() function (1,000,000 times) should take approximately the same amount of the time.
- For cache capacity 100, 10, 3, 2, 1, calling your *get*() function (1,000,000 times) should take approximately the same amount of the time. These cases take less time than the above cases, because they just return -1, but do not change the linked list; whereas the above cases require updating the linked list.

## Extra Testing

A bash script called *runval.sh* is provided to test your *cachefree*() function. The bash script utilizes a tool called valgrind to test if your memory is correctly released, if not, valgrind will report memory leaks. valgrind eventually will call lru-test6, thus before running this script, you need to run *make* so as to compile lru-test6.c and generate lru-test6. Once it is generated, the expected results are like the following:

```console
(base) [jidongxiao@onyx lru]$ ./runval.sh 
==603794== Memcheck, a memory error detector
==603794== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==603794== Using Valgrind-3.17.0 and LibVEX; rerun with -h for copyright info
==603794== Command: ./lru-test6
==603794== 
what we get from the cache are [4 3 2 -1 -1 2 3 -1 5]
==603794== 
==603794== HEAP SUMMARY:
==603794==     in use at exit: 0 bytes in 0 blocks
==603794==   total heap usage: 7 allocs, 7 frees, 81,168 bytes allocated
==603794== 
==603794== All heap blocks were freed -- no leaks are possible
==603794== 
==603794== For lists of detected and suppressed errors, rerun with: -s
==603794== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

As you can see, if your *cachefree*() function is implemented correctly, you should get zero errors and valgrind will report no memory leaks.

Note: if the runval.sh script you checked out is not an executable file, then you need to run "chmod +x runval.sh" to change it to an executable file.

## Submission

Due Date:  03/08/2022, 23:59pm. Late submission will not be accepted/graded.

## Project Layout

All files necessary for compilation and testing need to be submitted, this includes source code files, header files, the bash script, and Makefile. The structure of the submission folder should be the same as what was given to you. 

## Grading Rubric (for Undergraduate Students and Graduate Students)

Grade:  /100

- [10 pts] Compiling:
	- Each compiler warning will result in a 3 point deduction.
	- You are not allowed to suppress warnings.
- [75 pts] Functional Requirements:
	- lru-test1 produces correct results - /10
	- lru-test2 produces correct results - /10
	- lru-test3 produces correct results - /10
	- lru-test4 produces correct results - /10
	- lru-test5 produces correct results - /10
	- lru-test6 produces correct results - /10
	- O(1) time complexity - you get the pts if you lru-test7 produce similar results as the provided results. - /15
- [5 pts] Valgrind reports no memory leaks or errors:
	- As reported by *runval.sh*.
	- This is a PASS/FAIL score. 1 error or leaking 1 byte will result in a zero for this section. There is no partial credit for this section.
- [20 pts] Extra Credits:
	- You get 20 extra pts if your program passes all the test cases on Leetcode, see Leetcode problem 146. Note, you need to provide a screenshot in your submission to prove you have passed all the test cases on Leetcode. You will not get these extra points if such a screenshot is not included in your submission.
- [10 pts] Documentation:
	- README.md file (replace this current README.md with a new one using the README template, and do not check in this current README file.)
	- You are required to fill in every section of the README template, missing 1 section will result in a 2-point deduction.
