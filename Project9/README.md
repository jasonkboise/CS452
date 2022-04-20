## Project 9 - Toyota

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this project, I implemented a linux kernel module that serves
as a simple character device driver.

## Manifest

* Makefile - used to build the project
* toyota.c - implements the kernel module
* toyota.h - the header file for the main c file
* toyota-test[1-4] - testing files
* toyota_load - script file that loads the kernel module
* toyota_unload - script file that unloads the kernel module
* README.md - this file

## Building the project

In order to run this project, you must first navigate to the directory with 
all of the files, then run the command:

```
make
```
Then you must load the kernel module with the command:

```
sudo ./toyota_load
```
Then you can test the module with:
```
./toyota-test1
```

## Features and usage

For this project, we created a simple character driver that supports open, read, write and close operations.
When it writes, it stores the data in an internal buffer. When it later reads that
data, it removes duplicate letters from the string, so that every letter appears once and only once. 
It also makes sure the result is the smallest in lexicographical order among all possible results.
It then returns a stream of the result string based on how many bytes they ask for.


## Testing

I had to debug this project a lot. I was having a lot of memory management
issues, so I ended up using a lot of printk() statements in order to figure
out my issues. In order to find those issues, I would use the print statements
to print out what I had in each of my variables at different points of the
program. Using this, I found a lot of issues and one-by-one I fixed them.

## Known Bugs

N/A

## Reflection and Self Assessment

This project was pretty straight forward. I had a lot of confusion going into this
project on where to start. This was mostly because I was confused by the purpose of
the open() and release() functions. But once I read the provided README more carefully,
I realized how simple their implementation was.

The hardest part of the project was easily getting the read() function to work properly.
First, I had to get the deleteDuplicateLetters() functioning. Thankfully, the leetcode
explanation we had in class was really helpful. I also found some helpful discussion on
the leetcode question to help solve this issue. After that, all that was left was figuring
out how to return as a stream of bytes. Thankfully, this didn't turn out to be too complicated.

Throughout this whole process, my VM crashed a lot, and of course I had to have a bunch of
print statements everywhere to figure out some of my issues, but overall I didn't think
this project was too bad.

## Sources Used

* In class leet code solution
* https://leetcode.com/problems/remove-duplicate-letters/discuss/1861985/Stack-Explained
* In class whiteboard notes
* Provided README
