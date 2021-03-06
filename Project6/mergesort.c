/**
 * This file implements parallel mergesort.
 * Author: Jidong Xiao // Change this line to your name.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){

	int size = rightend - leftstart + 1;
	int left = leftstart;
	int right = rightstart;
	int i = leftstart;

	while (left <= leftend && right <= rightend) {
		if (A[left] <= A[right]) {
			B[i] = A[left];
			left++;
		}
		else {
			B[i] = A[right];
			right++;
		}
		i++;
	}

	memcpy(&B[i], &A[left], (leftend-left+1)*sizeof(int));
	memcpy(&B[i], &A[right], (rightend-right+1)*sizeof(int));
	memcpy(&A[leftstart], &B[leftstart], size*sizeof(int));
}

/* this function will be called by parallel_mergesort() as its base case. */
void mergesort(int left, int right){
	//if there is only 1 thing in list, it is sorted
	if (left >= right) {
		return;
	}
	int middle = (left + right)/2;
	mergesort(left, middle);
	mergesort(middle+1, right);
	merge(left, middle, middle+1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
	pthread_t t1, t2;
	struct argument *arg1, *arg2, *orig;

	orig = (struct argument*)arg;
	if (orig->level == cutoff) {
		mergesort(orig->left, orig->right);
		return NULL;
	}

	if (orig->left >= orig->right) {
		return NULL;
	}

	int middle = (orig->left + orig->right)/2;
	arg1 = buildArgs(orig->left, middle, orig->level+1);
	arg2 = buildArgs(middle+1, orig->right, orig->level+1);
	pthread_create(&t1, NULL, parallel_mergesort, arg1);
	pthread_create(&t2, NULL, parallel_mergesort, arg2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	merge(arg1->left, arg1->right, arg2->left, arg2->right);
	free(arg1);
	free(arg2);

	return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
	struct argument *arg = (struct argument*)malloc(sizeof(struct argument));
	arg->left = left;
	arg->right = right;
	arg->level = level;
	return arg;
}

/* vim: set ts=4: */
