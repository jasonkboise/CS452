# Overview

In this assignment, we will create a thread-safe version of an existing list library. We will then test it using a provided solution to the producer-consumer problem, which is also known as the bounded buffer problem. Note this is NOT a kernel project, and you should just develop your code on onyx, not in your virtual machine. Submissions fail to compile or run on onyx, will not be graded.

## Learning Objectives

- Gain more experience writing concurrent code.
- Understand a well-known concurrent programming problem - the producer-consumer problem.
- Explore the pthread library, and learn how to use locks and condition variables.

## Book References

Read these chapters carefully in order to prepare yourself for this assignment:

- [Threads API](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf) 
- [Locks](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
- [Condition Variables](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)

## Starter Code

The starter code looks like this:

```console
(base) [jidongxiao@onyx cs452-thread-safe-library]$ ls -R
.:
include  Item.c  Item.h  lib  list  Makefile  pc.c  README.md  test-pc.sh  wrapper-library

./include:

./lib:

./list:
include  lib  Makefile  README.md  SimpleTestList.c

./list/include:
List.h  Node.h

./list/lib:
libmylib.a  libmylib.so

./wrapper-library:
Makefile  ThreadsafeBoundedList.c  ThreadsafeBoundedList.h
```

You will be modifying *ThreadsafeBoundedList.c*, which is located in the wrapper-library subfolder. You should not modify the *ThreadsafeBoundedList.h* file in the same folder.

A testing program, which is the solution to the producer-consumer problem, is provided in the starter code, called *pc.c*. You should not modify *pc.c*. To run this testing program, you just type *make* and run:

```console
(base) [jidongxiao@onyx thread-safe-library]$ ./pc 
Usage: ./pc <poolsize> <#items/producer> <#producers> <#consumers> <sleep interval(microsecs)>
(base) [jidongxiao@onyx thread-safe-library]$ ./pc 100 2000 5 5 1
5 producers 5 consumers 10000 items 100 pool size
I am producer thread 4 (with thread id = 7F2397F5A700)
I am producer thread 3 (with thread id = 7F2397759700)
I am producer thread 2 (with thread id = 7F2396F58700)
I am producer thread 1 (with thread id = 7F2396757700)
I am producer thread 0 (with thread id = 7F2395F56700)
I am consumer thread 4 (with thread id = 7F2395755700)
I am consumer thread 3 (with thread id = 7F2394F54700)
I am consumer thread 2 (with thread id = 7F2387FFF700)
I am consumer thread 1 (with thread id = 7F23877FE700)
I am consumer thread 0 (with thread id = 7F2386FFD700)
producer 0 finished
producer 1 finished
producer 2 finished
producer 3 finished
producer 4 finished
consumer 0 finished
consumer 1 finished
consumer 2 finished
consumer 3 finished
consumer 4 finished
 #items produced = 10000   #items consumed = 10000 
```
As can be seen, the testing program takes five arguments. The first argument is *poolsize*, which is the capacity of the doubly linked list - this is why the producer-consumer problem is also known as the bounded buffer problem - you can consider the doubly linked list as a buffer, whose size is limited. In the above example, from the command line, we specify the capacity of the doubly linked list as 100, so that means at most we can add 100 nodes into this list. The second argument describes how many items each producer will produce - we assume each producer will produce the same number of items. In the above example, we specify each producer to produce 2,000 items. The third argument specifies how many producer threads we want to create, and the fourth argument specifies how many consumer threads we want to create. In the above example, we set both to 5, but these two numbers do not need to be identical. For example, you can have 3 producers, and 4 consumers. The fifth argument is a sleep interval, we let producers and consumers sleep for this interval so as to simulate the time of producing or consuming. You can just set it to 1, which represents 1 microsecond.

In addition to the testing program, a bash script is also provided in the starter code, and it is called *test-pc.sh*. You should not modify *test-pc.sh*. This script will just call *pc* and test *pc* with various arguments so as to test it more comprehensively. You can run this script like this:

```console
(base) [jidongxiao@onyx cs452-thread-safe-library]$ ./test-pc.sh 
```

Note: if the test-pc.sh script you checked out is not an executable file, then you need to run "chmod +x test-pc.sh" to change it to an executable file.

## Specification

A doubly linked list library is provided. As shown in the starter code, *./list/lib/libmylib.a* is the provided library. It is pre-compiled, meaning that you can use the library but you have no access to its source code. Coming with the library are two of its header files: List.h and Node.h, both are located in the list/include subfolder.

This library does not support multiple threads, thus when a program with multiple threads attempts to access this library - manipulating the doubly linked list, there will be race conditions and the results may not be deterministic. Our goal in this assignment therefore is to make this library a thread-safe library. However, we do not have the source code of this library, which means we can not change the library directly, therefore we will build a wrapper library that creates the monitor version (a monitor consists of a lock and one or more condition variables) around the original library. We will also add an additional feature to bound the capacity of the list.

To wrap this library, you will modify the *ThreadsafeBoundedList.c* file in the wrapper-library subfolder to complete the implementation of the thread-safe list library. In the remainder of this README, we will refer to this thread-safe list library as your library. In other words, in this assignment, there are two libraries, one is the original doubly linked list library, which is not thread-safe, the other is your library, which is thread-safe.
 
The functions your library will provide are listed below. These are called your APIs, or your API functions, and these functions are declared in the *ThreadsafeBoundedList.h* file in the wrapper-library folder. More specifically, we declare the prototype of all these API functions in *ThreadsafeBoundedList.h*, and you implement them in *ThreadsafeBoundedList.c*.

```c
tsb_createList()
tsb_freeList()
tsb_addAtFront()
tsb_addAtRear()
tsb_removeFront()
tsb_removeRear()
tsb_finishUp()
```

**Warning**: *tsb_addAtFront*(), *tsb_addAtRear*(), *tsb_removeFront*(), *tsb_removeRear*() look alike, thus you may decide to copy/paste code among them. And that is how you introduce bugs into your code. During your development, if your program is not working, the first question you should ask yourself is, did you copy/paste code among these four functions?

## Pre-defined Data Structure

In ThreadsafeBoundedList.c, we define a structure (*struct tsb_list*)  which contains a pointer to the underlying list as well as additional variables to implement the monitor.

```c
struct tsb_list {
 struct list *list;
 int capacity;
 Boolean stop_requested;
 pthread_mutex_t mutex;
 pthread_cond_t listNotFull;
 pthread_cond_t listNotEmpty;
};
```

Explanation to each of the fields:

- *list*: this is the original list we want to wrap and protect.

- *capacity*: we set the capacity from the command line. In the testing program, this is called the *poolsize*.

- *stop_requested*: read the [Additional Function](#additional-function) section and see if you can figure out why this field is needed, and how you should use it.

- *mutex*: to make the list library thread-safe, all of its functions should be protected by a mutex. (exception: createList() does not need to be protected)

- *listNotFull* and *listNotEmpty*: these are two condition variables. If the list becomes full, then adding to the list shall block on a condition variable (i.e., *listNotFull*). If the list is empty, then removing from the list shall block on another condition variable (*listNotEmpty*).

## Data Structures Defined in the Original List Library

The original list library defines *struct node* in *list/include/Node.h*:

```c
typedef struct node * NodePtr;

struct node {
        void *obj;
        struct node *next;
        struct node *prev;
};
```

And it defines *struct list* in *list/include/List.h*:

```c
typedef struct list * ListPtr;

struct list {
  int size;
  struct node *head;
  struct node *tail;
  int (*equals)(const void *, const void *);
  char * (*toString)(const void *);
  void (*freeObject)(void *);
};
```

The internal of these two structures are not really important to you. But you need to be aware of the *size* field of the *struct list*, which gives you the current size of the list. Knowing the current size of the list tells you if the list is full or not, and if the list is empty or not.

Most of the functions you need to implement either take one single argument, which is *(struct tsb_list * list)*, or take two arguments, which are *(struct tsb_list * list, NodePtr nodeNode)*. The only exception is *tsb_createList*(). The prototype of *tsb_createList*() is:

```c
struct tsb_list * tsb_createList(int (*equals)(const void *, const void *),
                   char * (*toString)(const void *),
                   void (*freeObject)(void *),
                                   int capacity);
```

This looks very complicated, but it is mainly because the original *createList*() has the following prototype.

```c
struct list* createList(int (*equals)(const void *, const void *),
                        char *(*toString)(const void *),
                        void (*freeObject)(void *));
```

You do not really need to understand what these arguments are doing, but in your *tsb_createList*(), you can call *createList*() as following:

```c
createList(equals, toString, freeObject);
```

In other words, you just pass the first three arguments of your *tsb_createList*() to *createList*(). Note this above line is incomplete, because *createList*() returns a *struct list* type pointer, and you may want to use this return value, whereas the above line does not assign its return value to anything.

To see how you should call the other functions defined in the original list library, refer to *list/include/List.h*, from there you can see their prototypes.

## APIs

I used the following pthread APIs:

- pthread_mutex_init()
- pthread_mutex_lock()
- pthread_mutex_unlock()
- pthread_mutex_destroy()
- pthread_cond_init()
- pthread_cond_wait()
- pthread_cond_signal()
- pthread_cond_broadcast()
- pthread_cond_destroy()

For each pthread API, read its man page to find out how to use it. 

## More Details about the Testing Program (pc.c)

In *pc.c*, we simulate a size-bounded queue (implemented as a doubly-linked list with a limit on maximum size) being shared by multiple producers and multiple consumers running simultaneously. Suppose we have m producers and n consumers (where m and n are supplied as command line arguments). The initial main process creates a global queue data structure so all threads can access it. Then the main process creates m producer threads and n consumer threads.

We will fix the number of items each producer produces (as a command line argument). Then we will keep track of how many items each consumer actually consumed. At the end of the program, we print out the total number of items produced versus the total number of items consumed. These two numbers should be the same unless the program has race conditions.

The items (defined in Item.c and Item.h) are defined as a structure which contains an unique id (basically number the items 1, 2, 3, ... as they are entered in the queue) and also contains the index of the producer that created it.

When running *pc* to test your library, the producers randomly insert new items at the front or back of the queue. Similarly the consumers randomly remove items from the front or back of the queue.

## Additional Function

When all producers are finished producing and exited, it is possible that some consumers are still sleeping. The testing program therefore needs to wake up sleeping consumers (if any). As such, we need to provide a function so that the testing program can call it to wake up sleeping consumers. This function is called *tsb_finishUp*(). This function is currently called by the testing program, and therefore you need to implement it in your *ThreadsafeBoundedList.c*. This is the only function which you are required to implement but is not a wrapper of any function from the original library.

## Submission 

23:59pm, March 29th, 2022. Late submissions will not be accepted/graded. 

## Project Layout

All files necessary for compilation and testing need to be submitted, this includes source code files, header files, the original library, the bash script, and Makefile. The structure of the submission folder should be the same as what was given to you.

Note: github does not like empty folders, when you check in, some of your folders may be empty: after running *make clean*, you may not have anything in the folder *include* and the folder *lib*. To check in empty folders, make sure you have an empty file called *.gitkeep* inside the folder. Otherwise, your check-in of the empty folder may fail - and the git command may not even warn you. That is why in the starter code, we see the *.gitkeep* file in these two folders.

```console
(base) [jidongxiao@onyx cs452-thread-safe-library]$ ls -a include/
.  ..  .gitkeep
(base) [jidongxiao@onyx cs452-thread-safe-library]$ ls -a lib/
.  ..  .gitkeep
```

These two folders do not contain any meaningful files at first, but once you run *make*, your wrapper library will be copied into *lib/*, and the header file of your library, which is *ThreadsafeBoundedList.h*, will be copied into *include/*, because that is where the testing program expects them to be.

## Grading Rubric (for Undergraduate and Graduate students)

All grading will be executed on onyx.boisestate.edu. Submissions that fail to compile on onyx will not be graded.
                                                                                     
- [80 pts] Functional Requirements:
  - [40 pts] Manually test: testing program *pc* produces expected results, when testing with 5 producers, 5 consumers, poolsize of 100, and each producer produces 2,000 items.
  - [40 pts] Use the testing script: test-pc.sh produces expected results. 
  - Definition of expected results: the total number of items consumed is equal to the total number of items produced, and the testing program or script does not crash, does not get stuck. 
- [10 pts] Compiling
  - Each compiler warning will result in a 3 point deduction.
  - You are not allowed to suppress warnings.
- [10 pts] Documentation:
  - README.md file: replace this current README.md with a new one using the template. Do not check in this current README.
  - You are required to fill in every section of the README template, missing 1 section will result in a 2-point deduction.

Note: Running valgrind is not required anymore for this and future assignments.
