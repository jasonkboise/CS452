
CC=gcc

INCLUDE=include/
CFLAGS = -g -O -Wall -MMD  -I$(INCLUDE) -I./list/include/
LIBS=-lpthread

# use this line below if we want to use the libraries dynamically
# LIBS=-lpthread -Llib/ -lthreadsafe-mylib -L./list/lib/ -lmylib

all: library pc

library:
	cd wrapper-library/; make
	cd wrapper-library/; make install

# $@: The filename representing the target.
# $?: The names of all prerequisites that are newer than the target, separated by spaces.
pc: pc.o Item.o wrapper-library/ThreadsafeBoundedList.o list/lib/libmylib.a
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

# if we do not use $? in the above line, then we need to use the following line so as to list every prerequisite file.
# $(CC) $(CFLAGS) -o $@ pc.o Item.o wrapper-library/ThreadsafeBoundedList.o list/lib/libmylib.a $(LIBS)

-include *.d

clean:
	/bin/rm -f *.o pc *.d
	cd wrapper-library/; make installclean
