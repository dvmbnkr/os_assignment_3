/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"
#include <pthread.h>

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend)
{
    int s, m, k;
    s = leftstart;  // Start
    m = rightstart; // "Middle" or "Start of Right side"
    k = leftstart;  // Start of the temporary array to copy
    while (s <= leftend && m <= rightend)
    { // At the start
        if (A[s] <= A[m])
        {                    // If the element in the most left subarray is smaller or equal to the element in the most right subarray
            B[k++] = A[s++]; // Temporarily store the smallest compared value
        }
        else
        {
            B[k++] = A[m++]; // If it is not the case, then store the value from the right - which would be the smallest.
        }
    } // Repeat this until it reaches the end of either subarray which indicates it has compared all the elements in that subarray
    while (s <= leftend)
    {                    // If there are remaining elements in the left subarray -- Incase its not perfect half
        B[k++] = A[s++]; // Store them in the temporary array
    }
    while (m <= rightend)
    {                    // If there are remaining elements in the right subarray -- Incase its not perfect half
        B[k++] = A[m++]; // Store them in the temporary array
    }
    memcpy(&A[leftstart], &B[leftstart], (rightend - leftstart + 1) * sizeof(int)); // Copy the sorted elements back to the original array like advised on Prac Page.
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right)
{
    int mid;
    if (left < right)
    {
        mid = left + (right - left) / 2;  // Find the mid point
        my_mergesort(left, mid);          // Recursively sort the left half
        my_mergesort(mid + 1, right);     // Recursively sort the right half
        merge(left, mid, mid + 1, right); // Merge the two halves
    }
}

/* this function will be called by the testing program. */
void *parallel_mergesort(void *arg)
{
    struct argument *args = (struct argument *)arg; // Cast the void pointer to argument struct pointer
    int left = args->left;                          // Extract left, right and level from the struct
    int right = args->right;
    int level = args->level;

    if (level >= cutoff || left >= right) // Base case: if level exceeds cutoff
    {
        my_mergesort(left, right); // Call the sequential mergesort
        return;
    }

    int mid = left + (right - left) / 2;                                       // Find the mid point
    pthread_t leftThread, rightThread;                                         // Create two threads for left and right subarrays
    struct argument *leftArgs = buildArgs(left, mid, level + 1);               // Build arguments for left thread
    struct argument *rightArgs = buildArgs(mid + 1, right, level + 1);         // Build arguments for right thread
    pthread_create(&leftThread, NULL, parallel_mergesort, (void *)leftArgs);   // Create left thread
    pthread_create(&rightThread, NULL, parallel_mergesort, (void *)rightArgs); // Create right thread
    pthread_join(leftThread, NULL);                                            // Wait for left thread to finish
    pthread_join(rightThread, NULL);                                           // Wait for right thread to finish
    merge(left, mid, mid + 1, right);                                          // Merge the two sorted halves
    free(leftArgs);                                                            // Free the allocated memory for arguments
    free(rightArgs);

    return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument *buildArgs(int left, int right, int level)
{
    struct argument *newArg = malloc(sizeof(struct argument)); // Create a new argument struct called newArg
    newArg->left = left;                                       // set the values
    newArg->right = right;
    newArg->level = level;
    return newArg; // Return the new struct
}


// Examples:

// Example to illustrate what merge() does:
// Suppose A = [2, 5, 8, 1, 3, 7]
// Left subarray:  [2, 5, 8]
// Right subarray: [1, 3, 7]
// Indices: leftstart=0, leftend=2, rightstart=3, rightend=5
// Merge process:
// Compare 2 and 1 → take 1  → [1]               s=0, m=4, k=1
// Compare 2 and 3 → take 2  → [1, 2]            s=1, m=4, k=2
// Compare 5 and 3 → take 3  → [1, 2, 3]         s=1, m=5, k=3
// Compare 5 and 7 → take 5  → [1, 2, 3, 5]      s=2, m=5, k=4
// Compare 8 and 7 → take 7  → [1, 2, 3, 5, 7]   s=2, m=6, k=5
// Right side done → copy 8  → [1, 2, 3, 5, 7, 8] s=3, m=6, k=6
// Result after merge():
// [1, 2, 3, 5, 7, 8]
// Before merge:   [2, 5, 8 | 1, 3, 7]
// After merge:    [1, 2, 3, 5, 7, 8]

// Example of how my_mergesort() works:
// A = [14, 33, 27, 10, 35, 19, 42, 44]
// Split:
// [14, 33, 27, 10]             and             [35, 19, 42, 44]
// [14, 33] and [27, 10]                        [35, 19] and [42, 44]
// [14] [33] [27] [10]                         [35] [19] [42] [44]
// Call merge(0,0,1,1) → [14, 33]
// Call merge(2,2,3,3) → [10, 27]
// Call merge(0,1,2,3) → [10, 14, 27, 33]
// Call merge(4,4,5,5) → [19, 35]
// Call merge(6,6,7,7) → [42, 44]
// Call merge(4,5,6,7) → [19, 35, 42, 44]
// Call merge(0,3,4,7) → [10, 14, 19, 27, 33, 35, 42, 44]
// Final sorted array:
// [10, 14, 19, 27, 33, 35, 42, 44]

// Example of how parallel_mergesort() works:
// A = [14, 33, 27, 10, 35, 19, 42, 44], cutoff = 1
// Split into two threads:
// Thread 1 → [14, 33, 27, 10]
// Thread 2 → [35, 19, 42, 44]
// In Thread 1:
// [14, 33] and [27, 10]
// [14] [33] [27] [10]
// Call merge(0,0,1,1) → [14, 33]
// Call merge(2,2,3,3) → [10, 27]
// Call merge(0,1,2,3) → [10, 14, 27, 33]
// In Thread 2:
// [35, 19] and [42, 44]
// [35] [19] [42] [44]
// Call merge(4,4,5,5) → [19, 35]
// Call merge(6,6,7,7) → [42, 44]
// Call merge(4,5,6,7) → [19, 35, 42, 44]
// Main thread waits (pthread_join)
// Call merge(0,3,4,7) → [10, 14, 19, 27, 33, 35, 42, 44]
// Final sorted array:
// [10, 14, 19, 27, 33, 35, 42, 44]
