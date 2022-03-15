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

For this project, we created a multi-threaded version of mergesort. In order to
accomplish this, we created the function parrallel_mergesort() which basically
splits the given array into two smaller pieces, then creates two threads that
call parrallel_mergesort() on the two smaller lists. This keeps happening, until
the desired number of threads are created, and then mergesort() is called on the
smaller lists. Once they are all sorted, each thread is merged together until
all of the threads are combined, creating the original, now sorted list.

## Testing

I once again took advantage of the new VS Code debugger tool to test and debug
my code. Although it was much less useful this time around because of the nature
of multi-threading, so I mostly had to rely on my intuition to find the problems
in my code. And of course, I also took advantage of the testing C code provided
to make sure that the output was correct.

## Known Bugs

N/A

## Reflection and Self Assessment

I was worried that this project was going to be a lot harder than it actually
ended up being. I ended not struggling too much on this one because I think that
both the provided README and in class lectures ended up making this project a lot
easier to understand.

I made this project by starting from the top and working backwards. So I started
by implementing parallel_mergesort() first, which was pretty straight forward,
as the base case and creating new threads was not too complicated. When I got to
making mergesort(), I started getting a little more confused, as it had been a while
since I had actually had to implement merge sort. Thankfully, the README for this
project had a really useful YouTube video link provided that went step-by-step
on how to implement the algorithm.

The difficult part of the project was making sure that my algorithm was working properly.
Because the instruction were written in Java, I had to make sure that I was
properly converting to C code. This was a bit of a challenge when it came to using
memcopy(). I eventually realized that my problem was I was not multiplying the size
by the size of an int for the amount of data to copy. Also, I realized I had some
memory leaks from not freeing the arguments I malloc'd, so I had to fix that as well.
Overall, this was not too difficult a project.

## Sources Used

* In class whiteboard notes
* Provided README
* [Algorithms: Merge Sort](https://www.youtube.com/watch?v=KF2j-9iSf4Q)