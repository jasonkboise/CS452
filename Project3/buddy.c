/**
 * Implementation of a memory allocator based on the Buddy System.
 */
 
#include "buddy.h"

int buddy_init(void) { 
	//when you initalize, you will start with just one big chunk of memory of DEFAULT_MEM_SIZE,
	//and initialize a lot of empty lists, which will hold smaller chunks
	//then split it when the user uses buddy_malloc for a smaller size than the large chunk

	/*
	base = sbrk();
	
	int i;
	for (i = 0; i< 29; i++) {
		next <- & avai[i];
		prev <- & avai[i];
		kval <- i;
		tag <- -1;
	}
	*/

	base = sbrk(DEFAULT_MAX_MEM_SIZE);
	struct block_header *p = (struct block_header *)base;

	int i;
	for(i = 0; i<29; i++) {
		avail[i].next = &avail[i];
		avail[i].prev = &avail[i];
		avail[i].kval = i;
		avail[i].tag = UNUSED;
	}

	avail[29].next = p;
	avail[29].prev = p;
	avail[29].kval = 29;
	avail[29].tag = UNUSED;

	p->next = &avail[29];
	p->prev = &avail[29];
	p->kval = 29;
	p->tag = FREE;

	return TRUE;
}

void *buddy_malloc(size_t size)
{
	//first add the size of the header to the amount they want to allocate
	//int realsize = (char *)size + 24 (maybe?)
	//this is ACTUALLY what you are trying to allocate

	//start searching from lgsize
	/*
	for (i = lgsize; i <29; i++) {
		if list not empty (then there a block free to use!)
			break;
	}
	while (j > lgsize) {
		...
		return something;
	}
	return NULL;
	*/

	//everytime you split the big chunk, you need to cast to add a buddy header
	
	//return null if init() was not called first
	if (base == NULL) {
		return NULL;
	}
	
	int i, j, remaining;
	struct block_header *p, *p2, *p3, new;
	int lgsize = 0;
	int free = FALSE;
	//int sizeSave = size;

	//add size of header
	size += sizeof(struct block_header);
	//calculate lgsize
	size=size-1;
    while(size>0){
        size=size>>1;
        lgsize++;
    }

	if (lgsize > 29) return NULL;

	//look for available block, starting from lgsize
	for (i = lgsize; i<31; i++) {
		p = &avail[i];
		while (p->next->tag != UNUSED) {
			p = p->next;
			if (p->tag == FREE) {
				free = TRUE;
				break;
			}
		}
		if (free) break;
	}
	if (i == 30) return NULL;

	//save avail level
	j = i;

	//finding how much memory is in a block at found avail level
	remaining = DEFAULT_MAX_MEM_SIZE;
	while (i < 29) {
		remaining = remaining >> 1;
		i++;
	}

	while (lgsize < j) {
		//remove node p from avail[j]
		//divide remaining by half
		//decrement j
		//split the node (start of the left node is 'base', and start of right node address is 'base + remaining')
		//adjust left node's header, and add new header to right node
		//add both nodes to avail[j]


		p2 = p->prev;
		p3 = p->next;
		//if it is the only block in the list
		if (p->next == p2) {
			p2->next = p2;
			p2->prev = p2;
		}
		//if it is not the only block
		else {
			p2->next = p3;
			p3->prev = p2;
		}
		remaining = remaining >> 1;
		j--;
		
		new = (struct block_header *)(base + remaining);
		p->kval = j;
		new->kval = j;
		new->tag = FREE;
		
	}

	//lgsize 17
	//avail[19]

	return NULL;
	//pointer = (char *)pointer + 24 (the size of the header)
	//return pointer

}

void buddy_free(void *ptr) 
{
	//first do pointer = (char *)ptr - 24;
	//then start freeing from pointer
}

void printBuddyLists(void)
{
	int i, count;
	struct block_header *p, *save;
	count = 0;
	for (i = 0; i < 30; i++) {
		p = &avail[i];
		save = p;
		printf("List %d: head = %p", i, p);
		while (1) {
			p = p->next;
			if (p == save) {
				printf(" --> head = %p", p);
				break;
			}
			else {
				printf(" --> [tag=%d,kval=%d,addr=%p]", p->tag, p->kval, p);
				if (p->tag == FREE) count++;
			}
		}
		printf("\n");
	}
	printf("\nNumber of available blocks = %d \n", count);
}

/* vim: set ts=4: */
