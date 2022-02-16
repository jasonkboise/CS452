/**
 * Implementation of a memory allocator based on the Buddy System.
 */
 
#include "buddy.h"

int buddy_init(void) { 

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
	//return null if init() was not called first
	if (base == NULL) {
		return NULL;
	}
	
	int i, j, remaining;
	struct block_header *p, *p2, *p3, *new;
	int lgsize = 0;
	int free = FALSE;
	void *ret;

	//add size of header
	size += sizeof(struct block_header);
	//calculate lgsize
	size=size-1;
    while(size>0){
        size=size>>1;
        lgsize++;
    }

	//if requesting too much memory, return NULL
	if (lgsize > 29) return NULL;

	//look for available block, starting from lgsize
	for (i = lgsize; i<30; i++) {
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
	//if nothing found return NULL
	if (!free) return NULL;

	//if the free block is at avail[lgsize], just return that block
	if (p->kval == lgsize) {
		struct block_header *t1, *t2;
		p->tag = RESERVED;
		t1 = p->next;
		t2 = p->prev;
		t1->prev = t2;
		t2->next = t1;
		ret = (void *)((char *)p + sizeof(struct block_header));
		return ret;
	}

	//save avail level
	j = i;

	//finding how much memory is in a block at found avail level
	remaining = DEFAULT_MAX_MEM_SIZE;
	while (i < 29) {
		remaining = remaining >> 1;
		i++;
	}

	while (lgsize < j) {
		/* remove node p from avail[j] */
		if (p->next != NULL && p->prev != NULL) {
			p2 = p->prev;
			p3 = p->next;
			
			p2->next = p3;
			p3->prev = p2;

			//remove p from all lists, as it is being used.
			p->next = NULL;
			p->prev = NULL;
			p->tag = RESERVED;
		}

		remaining = remaining >> 1;
		j--;
		
		//make new header half with into p to split it.
		new = (struct block_header *)(base + remaining);

		//adjust p and new's kval
		p->kval = j;
		new->kval = j;
		new->tag = FREE;

		/* now add 'new' to the next list */
		p2 = &avail[j];
		p3 = p2->next;
		
		p2->next = new;
		new->prev = p2;
		new->next = p3;
		p3->prev = new;
		
	}

	//add the size of the block_header to p to move the pointer past the header
	ret = (void *)((char *)p + sizeof(struct block_header));

	return ret;

}

void buddy_free(void *ptr) 
{
	struct block_header *p1;
	struct block_header *p2;
	p1 = (struct block_header *) ((char *)ptr - sizeof(struct block_header));
	//printf("p1 address: %p\n", p1);
	unsigned long long ull = 1;

	long x, y;
	x = (long)p1 - (long)base;

	//find buddy address
	y =(unsigned long long)(x) ^ (ull << (p1->kval));
	p2 = (struct block_header *)((long)p1 + (long)y);
	
	while (p2->tag == FREE && p2->kval == p1->kval) {
		
		//remove p2 from the list
		struct block_header *temp1, *temp2;
		temp1 = p2->prev;
		temp2 = p2->next;
		temp1->next = temp2;
		temp2->prev = temp1;

		//increment p1's kval to move up the list and find the next buddy
		p1->kval = (short)p1->kval + 1;

		//find buddy address
		y =(unsigned long long)(x) ^ (ull << (p1->kval));
		p2 = (struct block_header *)((long)p1 + (long)y);
	}

	if (p1 > p2 && p1->kval != 29) {
		p1 = p2;
	}
	
	p1->tag = FREE;
	struct block_header *temp = avail[p1->kval].next;
	avail[p1->kval].next = p1;
	temp->prev = p1;
	p1->prev = &avail[p1->kval];
	p1->next = temp;
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
