## Project Number/Title 

* Author: Your Name
* Class: CS452/CS552 [Operating Systems] Section #001

## Overview

In this assignment, we wrote a Linux kernel module called lexus. This module
implements a lottery scheduler for processes by maintaining a seperate scheduler
and dispatching processes to the CFS scheduler when we want them to run.

## Manifest

* Makefile - used to build the project
* lexus.c - the main C file that makess the kernal module implementing lottery scheduling
* lexus.h - header file for lexus.c that defines structs, functions, etc.
* lexus-test(1-3).sh - bash shell script files that test the lottery scheduling system
* README.md - this file

## Building the project

In order to run this project, you must first navigate to the directory
with all of the files, then run the command:

```
make
```
then, when the build is finished, run the command:

```
sudo insmod lexus.ko
```

finally, run you can run a test script by running the command:
```
./lexus-test1.sh
```

## Features and usage

The project is a my own implementation of a lottery scheduling system. This
is accomplished by maintaing a list of processes, each with a certain amount
of tickets (to give them a priority), and holding a lottery to see which process
gets some processing time on a one-core CPU system. In order to add a process
to the lottery system, I had to intercept ioctl() call to check when a process
wanted to be entered into the CFS, which is the default linux scheduler. However, 
instead of adding it to the CFS, it is added to my own scheduler first. If the 
process wins the lottery, I then add it to the CFS with high priority so that it 
gets processed for a set amount of time. Then after that set amount of time, I
set the priority of the previous winner back to 0, then hold another lottery,
and set the next winner to high. It then repeats the process until all processes
in the lottery scheduler are complete. 

## Testing

I debugged this project is much the same way as the first project for this class.
So I started out by using a bunch of printk() statements to figure out exactly
what a certain perameter for a function was, if some structs were getting passed
to register/unregister correctly, etc. Obviously, this meant that my VM ended up
crashing a bunch of times, as this method still isn't super efficient compared
to using GDB. But, I still felt like I was making progress using this method, so
I didn't feel it was super necessary for me to follow the guide on setting it up
and using it. I will probably end up using it on a future project though.

## Known Bugs

N/A

## Reflection and Self Assessment

Now that I have had some time getting used to making a kernel module, I found
this project to still be challenging, but oddly not as difficult as the first project.
This is most likely just because I have a better understanding of how to go about
debugging the code. Also, I found the README provided for the project to very useful
starting point for pretty much all of the functions.

So as the README suggested, I started by implementing the ioctl() function. I started
working on the project after our class on Thursday, so I had a pretty good idea of where
to start for this function. We basically just had to check if the ioctl perameter was
LEXUS_REGISTER or LEXUS_UNREGISTER, and based on that, pass it to register() or
unregister(). The only difficult part of this was remember how to properly use
copy_from_user() for the args perameter to get the lottery_struct. But once I had this
figured out, I had a lot of printk() statements to make sure it was being properly
passed to register() and unregister(). 

Register() and unregister() were pretty straightforward. In register() I just
instatiated a lexus_task_struct, filled it with the info passed in from ioctl(), and
added that to our global list. And then unregister() was just the opposite, where
I freed the lexus_task_struct and deleted it from the list when the process was done.
And of course, I had printk() statements everywhere making sure everything was working
as intended. I encountered an issue though where removing the module would cause the
VM to crash. I could not find the reason as to why this happened, but after I finished
schedule() this seemed to go away, so I assume it had something to do with that.

Schedule() easily caused the most frustration. I started out by basically following
the psuedo code provided by the textbook. From there, I started adding in lines of
code from the README to schedule/unschedule the winner/previous winner from the CFS.
After this was still causing crashing, I realized it wasn't enough. The main thing I
realized was missing was the while loop that the entire code is suppose to be surrounded
by, as I remembered that the professor mentioned in class that we want this code to
keep running forever until the whole list is done. After this point, my code was
mostly working. However, I was running into an issue where the module would cause my
VM to crash if I ran a script more than once. To fix this, I realized that I was
calling spin_lock_irqsave() too early, which lead to issues if I locked it, and then
the loop was reset if the list was empty. So once I fixed this, my code was working
completely.

Overall, this was a challenging, but also interesting project. Learning how the lottery
system works was cool, so I found programming this not too annoying.

## Sources Used

* In class whiteboard notes
* Operating Systems: Three Easy Pieces: Chapter 9: Lottery Scheduling
* https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch10s05.html