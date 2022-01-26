## Project 1: Tesla

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section 001

## Overview

In this assignment, we wrote a Linux kernel module called tesla. This module
basically overwrites some common system calls and replaces them with our own
implementations.

## Manifest

-Makefile: used to build the project
-tesla.c: the main C file that makes the kernal module
-tesla.h: Header file for tesla.c
-README.md: this file

## Building the project

In order to run this project, you must first navigate to the directory
with all of the files, then run the command:

```
	make
```
then, when the build is finished, run the command:

```
	sudo insmod tesla.ko
```


## Features and usage

This project is my own implementation of the common system calls 'write' and
'getdents.' The goal was to make them function mostly the same with some
exceptions. For the 'write' system call, when 'write' is called, it does not
print out any processes with the word 'ssh.' So for example, if a user has the 
kernel module installed and tries to use the command:

```
	ps -ef | grep ssh
```
Nothing should be found even if there are ssh processes running.
A similar idea was used for my implementation of getdents. If there is a file
with the word 'tesla' in it, ignore it. This means that if a user tries to use
the command

```
	ls -la
```
No files with 'tesla' in it will be displayed even if they actually exist in the
directory.

## Testing

I did not end up using KGDB for this project. Instead, my testing method came down
to a lot of printk() statements scattered throughout my code. Sometimes it was
to print addresses, or the value of 'ret,' or making sure that copy_from_user()
and copy_to_user() properly returning 0. Then, to monitor my print statements,
I had another terminal open with the following command running:

```
	sudo tail -f /var/log/messages
```


## Known Bugs

I'm not completely sure, but I think that sometimes when I have the kernel module
installed and I run 'ls' a bunch of times, it sometimes crashes the VM. This is
very inconsistent though, so I'm not sure if this is a problem with the program
or my VM.

## Reflection and Self Assessment

This was a really difficult project for me, mostly because of the very tedious
debugging process. However when I first started, early on I didn't really have much
clue what I was doing. This was mostly because I did not fully grasp the concept
of user and kernel spaces and how they interact. It wasn't until the second day
of class that week when the professor went over some pseudo code for the project
that things started to make sense for me. I now knew that you first need to make
a buffer specifically in kernel space and copy the info from the user buffer before 
you could start messing around with the contents of it. Once I understood this
concept, coding became easier.

So first, I started with the tesla_write() function. This one was pretty straight
forward, as I mostly was just following along my notes from class. First I made
the kernel buffer, copied the user contents over, then checked inside the buffer
for the word 'ssh' if the user was using the command 'ps' or 'grep.' If they were,
I simply returned the original count, ignoring the buffer. If it didn't have the
word, I returned the original system call. During this coding process, I think
I had some crashing issue, but I think it mostly came down to missing some things
from the pseudo code, then adding it in and restarting.

The very difficult part came from when I got to tesla_getdents(). This was frankly
a nightmare for me to figure out. In retrospect, I probably should have stopped
at this point to figure out how to install KGDB. However, I am very stubborn. So
first, I started out my following the structure laid out in the pseudo code from 
class. However, there were a few things wrong off the bat. First, I realized that I
should use the return value from the orig_getdents() as the size of my buffer, as
it returns the number of bytes read, A.K.A the size of the list of structs. Next,
I worked on the while loop. The hard part of this was figuring out memmove().
I had to draw out actual pictures in paint to understand exactly how I wanted to
shift the memory to overwrite the struct if it found the name 'tesla.' Eventually,
I came up with the first two parameters, and was really struggling with getting the
third one right. I originally thought it needed to be (ret - something) because you
don't need to write for the entire length of the list in bytes. However, I eventually
found that just putting ret worked. To be honest, I'm still not sure how it works,
but it gives the desired effect so I will not question it.

Throughout this whole process, my VM crashed probably a 100 times at least. This lead
to a lot of frustration and guessing as to how it went wrong. This is because
unfortunately, if the VM crashes, your printk messages do not get written. So basically,
my process was to comment out many lines of code so it wouldn't crash, then write a
bunch of printk() statements to figure out what was going wrong, then slowly uncommenting
code to see which parts were causing the issues. This was a long and tedious process,
but eventually I got it working, so I am happy with how it turned out.

## Sources Used

-In class psuedocode
-Memmove examples from online:
https://www.geeksforgeeks.org/memmove-in-cc/
https://www.ibm.com/docs/en/i/7.1?topic=functions-memmove-copy-bytes