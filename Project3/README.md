## Project 3 - Porsche

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this assignment, we wrote a C program that implements a buddy malloc and freeing
system.

## Manifest

* Makefile - used to build the project
* buddy.c - the main C file that implements the buddy malloc/free system
* buddy.h - header file for buddy.c that defines structs, functions, etc.
* buddy-test - testing file
* buddy-unit-test - testing file
* malloc-test - testing file
* README.md - this file

## Building the project

In order to run this project, you must first navigate to the directory
with all of the files, then run the command:

```
make
```
then, you can try testing the buddy malloc/free system by using

```
./buddy-unit-test v
```

## Features and usage

This project is my own implementation of the buddy malloc system. Basically,
the idea is that the system maintains a list of lists of open memory blocks.
Each list is in order of k-value, i.e. the size of the blocks in the list.
When a user asks for a certain amount of memory, the system finds the next
open block, and splits the blocks repeatedly if the block is too big for how
much the user is requesting. Each block created from the split is added to their
appropriate list. The free function then does the opposite. The user gives a
block back, and the system builds the blocks back up until there are no buddy
blocks available to add back together.

## Testing

For this project, I took advantage of buddy-unit-test.c a lot. It was really
useful for checking how my malloc() was working. In order to gather more info
on exactly how the function was working, I also used a lot of print statements.
I also practiced more with using GDB and found it reallt useful.

## Known Bugs

For some reason, when using buddy-test for over a certain amount of calls, it causes
a segment fault. This happens because the kval, tag, prev, and next values somehow
get corrupted at some point for some blocks during malloc(). Not sure why this
happens but I tried to fix it for many hours with no luck.

## Reflection and Self Assessment

This project was really hard. The difficulty mostly came from trying to understand
conceptually what I was doing. It took going to all the lectures and having in-class
discussions for me to understand not only what I was doing, but also generally how
I should go about it. I basically took the project in baby steps, but working on it
little by little over the few weeks we had.

Init was straightforward, I just needed to figure out how to cast the block_header to
the front of the block of memory, then the rest was simple. Malloc was a lot harder.
It took me about half a week to figure out, and lots of testing. Especially the while
loop at the end took lots of debugging.

Free was where things got really difficult. I really had a hard time trying to understand
this conceptually and practically. It wasn't until the lecture we had specifically
about free() where it started to make more sense to me. The part that was causing me
trouble was understanding how to find the buddy of a block of memory. Thankfully the
page on the canvas was helpful in this regard. This took me a whole week to implement.

I was really happy when my unit tests were finally all passing. The problems came back
when I tried using buddy-test. Running it many times would sometimes cause a segment
fault and I tried really hard to debug it, with no luck. I'm pretty sad I couldn't
figure this one out. Other than that, I am still pretty happy with how I did on this
project.

## Sources Used

* In class whiteboard notes
* Class canvas page "buddy calculation"
* Operating Systems: Three Easy Pieces: Chapter 17: Free Space Management