## Project 9 - Audi

* Author: Jason Kuphaldt
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this project, I implemented a linux kernel module that serves
as a simple file management system.

## Manifest

* Makefile - used to build the project
* test/ - folder used for testing
* audi.h/bitmap.h - header files that defines many functions used in this project
* audi_main.c - the audi file system kernel module
* dir.c - implements directory handlers.
* file.c - implements file creation.
* inode.c - handles the bash commands for making files
* mkfs.c - implements the application utility which allows us to format a disk partition
* super.c - implements functions to handle the superblock.
* README - this file

## Building the project

In order to run this project, you must first navigate to the directory with 
all of the files, then run the command:

```
make
```

Then you must create a file system image with 
```
dd if=/dev/zero of=test.img bs=4K count=64
```

Then you must create the file system layout with
```
./mkfs.audi test.img 
```

next, you install the kernal module with
```
sudo insmod audi.ko
```

then finally, mount the file system onto the test folder with
```
sudo mount -o loop -t audi test.img test
```

Navigate to the test folder and try to touch and mkdir files/directories.


## Features and usage

For this project, we created a Linux kernel module called audi. This module serves as a file
system whose layout matches the file system presented in the book chapter [File System Implementation](https://pages.cs.wisc.edu/~remzi/OSTEP/file-implementation.pdf).
Basically the module allows the user to create files, directories, look them up, and eventually
delete them. 


## Testing

To test this project, I manually tested the bash commands in the test folder after installing
my file system to see if they worked. Sometimes they would act in strange ways, so I take advantage
of printk() and pr_info() statements to figure out what was going wrong with my program.

## Known Bugs

There seems to be an issue where if the user tries to make a directory in an inode number that
was previously used by a different file, they are unable to access that folder. I was not able
to fix this issue in time before turing this project in.

## Reflection and Self Assessment

This project was honestly a pain. Going in, I didn't think it seemed too difficult. We were given
pretty easy to follow directions, and there were lots of hints given in the README for how to go
about creating this project. However, I hit many, many roadblocks along the way, and some that I
never could figure out the solution too. So I basically just took it one function at a time.

Create wasn't too bad to make, and was pretty straight forward, as was lookup. Of course there was
lots of crashing and headscratching moments along the way, but I was eventually able to solve them.
The real problem came from unlink. I must have read the README a million times, and tried a bunch of
different alternative ways of going about it, but was still having the bug mentioned in the previous
section. I ultimately decided that I was okay with getting partial credit on this project, but still
a little disappointed that I couldn't figure it out. Oh well!

## Sources Used

* In class leet code solution
* In class whiteboard notes
* Provided README
