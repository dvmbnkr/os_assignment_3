/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	int s,m,k;
	s = leftstart; // Start
	m = rightstart; // "Middle" or "Start of Right side"
	k = leftstart; // Start of the temporary array to copy
	while(s <= leftend && m <= rightend){ // At the start
		if(array[s] <= array[m]){ // If the element in the most left subarray is smaller or equal to the element in the most right subarray
			temp[k++] = array[s++]; // Temporarily store the smallest compared value
		}else{
			temp[k++] = array[m++]; // If it is not the case, then store the value from the right - which would be the smallest. 
		}
	} // Repeat this until it reaches the end of either subarray which indicates it has compared all the elements in that subarray
	while(s <= leftend){ // If there are remaining elements in the left subarray -- Incase its not perfect half
		temp[k++] = array[s++]; // Store them in the temporary array
	}
	while(m <= rightend){ // If there are remaining elements in the right subarray -- Incase its not perfect half
		temp[k++] = array[m++]; // Store them in the temporary array
	}
	memcpy(&array[leftstart], &temp[leftstart], (rightend - leftstart + 1) * sizeof(int)); // Copy the sorted elements back to the original array like advised on Prac Page. 

}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	int mid;
	if (left < right){
		mid = l + (right - left) / 2; // Find the mid point
		my_mergesort(left, mid); // Recursively sort the left half
		my_mergesort(mid + 1, right); // Recursively sort the right half
		merge(left, mid, mid+1, right); // Merge the two halves
	}
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
		struct argument *newArg = malloc(sizeof(struct argument)); // Create a new argument struct called newArg
		newArg -> left = left; // set the values
		newArg -> right = right;
		newArg -> level = level;
		return newArg; // Return the new struct
}

