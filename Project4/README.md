## Project 4 - Infiniti

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this assignment, we wrote a Linux kernel module that implements a page fault 
handler.

## Manifest

* Makefile - used to build the project
* fault.c - the main C file that implements the page fault handler
* fault.h - header file for fault.c
* infiniti_main.c - C file used for testing purposes
* infiniti.h - header file that has more useful functions
* user/ - contains all of the testing files
* README.md - this file

## Building the project

In order to run this project, you must first navigate to the directory with 
all of the files, then run the command:

```
make
```
Then to install the kernal module, you must use the command:

```
sudo insmod infiniti.ko
```


In order to test the module, you navigate to the user folder, and run the 
following to build the test files:

```
make
```
then try running one of the tests by using:

```
./infiniti-test1
```

## Features and usage

When a page fault happens, a kernel level function will be called to handle it, 
and this function is known as the page fault handler. For this project, we
created our own page fault handler as a kernel project. To do this, we had to
implement two different functions: infiniti_do_page_fault() and infiniti_free_pa().
The first function is called when the application triggers a page fault, so
the function asks for physical memory from the kernel and then updates the page
tables. The second function is called when the application calls infiniti_free().
This function gives the physical memory back to the kernel and then updates
the page tables.

## Testing

For this project, we were given a lot of help via psuedocode provided in class, so 
I did not need to go too deep into testing to make sure it worked. All I needed
was some printk() statements to make sure everything was functioning properly, and
then using the provided test files to make sure my ouput was correct. I actually
did not have too many crashes for this project.

## Known Bugs

N/A

## Reflection and Self Assessment

This project probably would have been a lot more difficult without all of the
help that we recieved in class. Having some psuedocode to follow, as well as
the detailed README, made this project not too bad.

The hardest part was definitely the first function, infiniti_do_page_fault().
Conceptually I understood what I was doing with the bit manipulation, but I
was still getting some weird errors when I kept stepping through each table.
I came to realize that most of my issues came down to casting properly, and
making sure everything was uniform so that addresses were being obtained
correctly.

The free function was pretty easy. Once I figured out how to step through each
of the tables and their entries, doing it again for free was very simple.
Then, I just followed the directions set out in the README, and getting the
function working wasn't too bad. So overall, I thought the project went
surprisingly smooth.

## Sources Used

* In class whiteboard notes