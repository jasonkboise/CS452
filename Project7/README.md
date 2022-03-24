## Project 7 - Dodge

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this project, I implemented a thread-safe version of an existing
list library.

## Manifest

* Makefile - used to build the project
* list/ - contains the list library
* wrapper-library/ - contains the thread-safe version of the list
* Item.c - defines an Item for the producers/consumers
* Item.h - the header file for Item.c
* pc.c - the testing file for this project
* test-pc.sh - a script file to run the testing file in many ways
* README.md - this file

## Building the project

In order to run this project, you must first navigate to the directory with 
all of the files, then run the command:

```
make
```
Then try running a test by using the command:

```
./pc [poolsize] [#items/producer] [#producers] [#consumers] [sleep interval]
```

## Features and usage

For this project, 

## Testing

This project came together pretty quickly, so I did not require a lot of testing
for this one. I mainly just used the testing files provided, both pc.c and
test-pc.sh.

## Known Bugs

N/A

## Reflection and Self Assessment

Much like how we discussed in class, this was definitely an easy assignment.
It definitely seemed much more complicated to me on paper, having to deal with
scary sounding things like a mutex or condition variables. But they ended up
being a lot less complicated than I thought they would be. The thing that helped me understand the concepts the most was when we did the
leet code problem in class. This basically covered all of the aspects of the
project that we had to know, so I referenced that solution a lot. 

The only part that was a bit tough for me was understanding the finishUp() function.
I eventually figured out that using the broadcast function, along with
changing the stop_requested integer allowed the consumers to finish their
job.

## Sources Used

* In class leet code solution
* In class whiteboard notes
* Provided README