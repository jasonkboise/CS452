## Project 6 - Mazda

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this project, I implemented a multi-threaded version of the merge sort 
algorithm that splits an array into multi parts that is each sorted in its
own thread for faster run times.

## Manifest

* Makefile - used to build the project
* mergesort.c - the main C file that implements the merge sort
* mergesort.h - the header file for the main C file
* runval.sh - a bash script file for testing for memory leaks
* test-mergesort.c - a testing file
* README.md - this file

## Building the project

In order to run this project, you must first navigate to the directory with 
all of the files, then run the command:

```
make
```
Then try running a test by using the command:

```
./test-mergesort [size of array] [# of threads] [randomizer seed]
```

## Features and usage

For this project, we had to create a LRU policy cache. This cache has a O(1) time
complexity by making use of a hash table. We did this by first creating the cache
out of a struct that has a size, capacity, hash table, and a head/tail pointing
to nodes. These nodes contain a key, value, and a next/prev pointer. So the cache
contains a doubly linked list of nodes that also go in the hash table to make
searching for the proper nodes much quicker. Also any nodes that are added/editted
are moved to the tail of the list.


## Testing

In order to test this project, I made use of the given test files a lot. Although
their output was useful to see where I went wrong, their real help came from
when I used them in a debugging tool built into the VS Code IDE. When I ran
the test files in a debugger, I could see exactly how things were being moved
around or removed from the cache. This was really helpful, and made development
of this project a breeze. I also used the bash script file a lot to remove
any memory leaks.

## Known Bugs

N/A

## Reflection and Self Assessment

The development for this project went really smoothly. After our class discussion
of a leet code problem that also similarly used a hash table, I had a firm understanding
of what exactly I needed to do for the project.

So I ended up coding most of free(), get(), and put() first, then started debugging using
the test files and the debugger tools in VS Code. Through using that, I found a lot of
edge cases that I was not accounting for. For example, I wasn't properly removing
things from the hash is the size reached the capacity. Also, my get function was breaking
if there was only one thing in the hash, so I had to account for that as well. If I
had not used the debugger, this project would have been a lot more difficult, as I couldn't
have followed exactly what my code was doing.

After I had those three functions working, all I had was free(), which went over pretty
easily as well. I basically followed the method of removing things from the hash that
I was using before in put(), but for everything in the list. I used the bash script
file we were given to also test to make sure this function was working properly. So
overall, this project went over well.

## Sources Used

* In class whiteboard notes
* Textbook Chapters 19 & 22