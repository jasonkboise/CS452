CC = gcc
AR = ar
CFLAGS = 

LIB_OBJS = lru.a

OBJS =  lru-test1 lru-test2 lru-test3\
	lru-test4 lru-test5 lru-test6\
	lru-test7\

build = \
	@if [ -z "$V" ]; then \
		echo '	[$1]	$@'; \
		$2; \
	else \
		echo '$2'; \
		$2; \
	fi

% : %.c $(LIB_OBJS)
	$(call build,LINK,$(CC) $(CFLAGS) $< $(LIB_OBJS) -o $@)

%.o : %.c 
	$(call build,CC,$(CC) $(CFLAGS) -c $< -o $@)

%.a : %.o
	$(call build,AR,$(AR) rcs $@ $^)


all: $(LIB_OBJS) $(OBJS)


clean: 
	rm -f *.o $(OBJS) $(LIB_OBJS)
