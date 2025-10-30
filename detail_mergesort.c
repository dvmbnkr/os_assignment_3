/**
 * This file implements parallel mergesort.
 */

// Updating

#include <stdio.h>      // Standard I/O header (used only for optional debugging/printing)
#include <string.h>     // For memcpy() — used to copy sorted data back to main array
#include <stdlib.h>     // For malloc() and free()
#include "mergesort.h"  // Contains definitions of global variables (A, B, cutoff) and struct argument
#include <pthread.h>    // POSIX threads library for creating and joining threads

/* -----------------------------------------------------------------------------
 * Function: merge
 * Purpose : Merges two sorted subarrays back into one sorted array.
 * Called by both my_mergesort() and parallel_mergesort() once two halves are sorted.
 * ----------------------------------------------------------------------------- */
void merge(int leftstart, int leftend, int rightstart, int rightend)
{
	int s, m, k;                     // Temporary index variables
	s = leftstart;	// s points to the beginning of the left subarray
	m = rightstart; // m points to the beginning of the right subarray
	k = leftstart;	// k marks where to insert into temporary array B

	// Loop as long as both subarrays have unmerged elements
	while (s <= leftend && m <= rightend)
	{
		// Compare elements from left (A[s]) and right (A[m])
		if (A[s] <= A[m])
		{						
			B[k++] = A[s++]; // Copy from left if smaller or equal, then increment both indices
		}
		else
		{
			B[k++] = A[m++]; // Otherwise copy from right side, increment right index
		}
	}

	// If any elements remain in the left subarray, copy them all into B
	while (s <= leftend)
	{					 
		B[k++] = A[s++]; 
	}

	// If any elements remain in the right subarray, copy them all into B
	while (m <= rightend)
	{					 
		B[k++] = A[m++]; 
	}

	// Copy the merged sorted portion from temporary array B back into main array A
	// (rightend - leftstart + 1) calculates the total number of merged elements
	memcpy(&A[leftstart], &B[leftstart], (rightend - leftstart + 1) * sizeof(int));
}

/* -----------------------------------------------------------------------------
 * Function: my_mergesort
 * Purpose : A standard single-threaded recursive merge sort.
 * Called as the base case in parallel_mergesort() when cutoff is reached.
 * ----------------------------------------------------------------------------- */
void my_mergesort(int left, int right)
{
	int mid;
	if (left < right) // Base condition: at least two elements in subarray
	{
		mid = left + (right - left) / 2;  // Find midpoint safely (avoids overflow)
		my_mergesort(left, mid);		  // Recursively sort left half
		my_mergesort(mid + 1, right);	  // Recursively sort right half
		merge(left, mid, mid + 1, right); // Merge the two sorted halves
	}
}

/* -----------------------------------------------------------------------------
 * Function: parallel_mergesort
 * Purpose : Recursive parallel implementation of merge sort using pthreads.
 * Logic   :
 *   - If current level < cutoff → create two new threads for left & right halves.
 *   - Otherwise → switch to normal my_mergesort().
 * ----------------------------------------------------------------------------- */
void *parallel_mergesort(void *arg)
{
	struct argument *args = (struct argument *)arg; // Cast the generic void pointer to our argument struct
	int left = args->left;							// Extract left boundary index
	int right = args->right;						// Extract right boundary index
	int level = args->level;						// Extract current recursion level (depth)

	// BASE CASE: stop creating threads when cutoff is reached or subarray is too small
	if (level >= cutoff || left >= right)
	{
		my_mergesort(left, right); // Perform standard mergesort instead of creating threads
		return;                    // Return to terminate this thread (no more recursion)
	}

	// Compute midpoint of current subarray
	int mid = left + (right - left) / 2;									   

	// Declare thread identifiers for left and right recursive calls
	pthread_t leftThread, rightThread;										   

	// Build separate argument structs for left and right threads
	struct argument *leftArgs = buildArgs(left, mid, level + 1);			   
	struct argument *rightArgs = buildArgs(mid + 1, right, level + 1);		   

	// Create two threads that each call parallel_mergesort() recursively
	pthread_create(&leftThread, NULL, parallel_mergesort, (void *)leftArgs);   
	pthread_create(&rightThread, NULL, parallel_mergesort, (void *)rightArgs); 

	// Wait until both threads finish (ensures both halves sorted before merging)
	pthread_join(leftThread, NULL);											   
	pthread_join(rightThread, NULL);										   

	// After both halves are sorted, merge them together
	merge(left, mid, mid + 1, right);										   

	// Free the dynamically allocated argument structs to prevent memory leaks
	free(leftArgs);															   
	free(rightArgs);

	return NULL; // Thread functions must return a void* (NULL here)
}

/* -----------------------------------------------------------------------------
 * Function: buildArgs
 * Purpose : Allocates and initializes a new struct argument for thread creation.
 * Called by parallel_mergesort() before each pthread_create() call.
 * ----------------------------------------------------------------------------- */
struct argument *buildArgs(int left, int right, int level)
{
	struct argument *newArg = malloc(sizeof(struct.argument)); // Allocate memory for struct
	newArg->left = left;									   // Assign left index
	newArg->right = right;									   // Assign right index
	newArg->level = level;									   // Assign recursion level
	return newArg; // Return the pointer to this struct so it can be passed to pthread_create()
}