#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#define CHAR_MAX 255

#define ALIGNMENT 8 // must be a power of 2

#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
/* For Alignment = 8 bytes:
// ALIGN(1) = 8 ,ALIGN(3) = 8, ALIGN(5) = 8, ALIGN(7) = 8
// ALIGN(9) = 16, ALIGN(11) = 16, ALIGN(13) = 16, ALIGN(15) = 16
// ALIGN(17) = 24, ALIGN(19) = 24, ALIGN(21) = 24, ALIGN(23) = 24
// ALIGN(25) = 32, ALIGN(27) = 32, ALIGN(29) = 32 , ALIGN(32) = 32 */

#define SIZE_T_SZ (ALIGN(sizeof(size_t))) // (header size type's size under memory alignment)
#define PREALLOCATED_HEAP_SIZE 16384
#define MIN_BLK_SZ (ALIGN(2)) // 2 Bytes due to using the lsb bit of the header as a flag 
#define SPLIT_TRESHOLD_DIVIDER 1 // 50% 

// address of heap start point in main memory = arr
char arr[PREALLOCATED_HEAP_SIZE] = { (char)NULL }; //TODO: should it be static variable?, externed?


long pow(int x, unsigned n)
{
	long long power = 1;

	for (int i = 0; i < n; i++) {
		power = power * x;
	}

	return power;
}

size_t* heap_start() {
	return (size_t *)arr;
}

size_t* heap_end() {
	return (size_t *)(arr + PREALLOCATED_HEAP_SIZE);
	//return &(arr[PREALLOCATED_HEAP_SIZE])  //identical
}
/* In order to reuse blocks, we must search the heap
// for a free block with size <= required size
// an implicit list based DMA  (first-fit algorithm)
// - navigating through blocks using sizes
// - O(n) search, where n = # blocks
// - n comprises allocated & free blocks! 		  */
void *find_fit(size_t size) {

	size_t* header = heap_start();
	size_t* next;

	while (header < heap_end()) {
		//check the "available" bit (is this block unallocated?)   
		if (!(*header & 1)) {
			if (*header >= size) // is the free block big enough
				return header;
			else {
				next = (size_t *)((char *)header + *header); //move to next adjacent block

				// merge with next block if available & free
				//may result in a cascade of merges during search
				// —> indeterminate performance
				if (next < heap_end() && !(*next & 1)) {
					*header += *next;
					continue; //try again after fusion
				}
			}

		}
		header = (size_t *)((char *)header + (*header & ~1L)); // skip this used block (pointer arithmetic)
													// ptr + #Bytes_to_Skip, L = long integer				
													// long - to support large address space size  
	}

	return NULL; //can't allocate required memory
}


void* malloc(size_t size) {
	// PLEASE NOTE : I didn't implement here best-fit mechanism or a modified version of it
	//               which will look results in minimal external fragmentation 

	// The user doesn't know/care we use larger memory for management
	size_t blk_size, treshold_size, new_size, *header;

	// largest offset inside the array(heap memory) for this implementation
	// is a little bit less than 32KB on typical 32bit systems 
	size_t max_block_size = (size_t)(pow(2.0, (SIZE_T_SZ - 1))) - 2 * SIZE_T_SZ;

	if (size < MIN_BLK_SZ || size > max_block_size)
		return NULL; //should notify the user what he done wrong

	blk_size = ALIGN(SIZE_T_SZ + size + SIZE_T_SZ); // [header|  user data  | tail(header's copy) ]	
	header = find_fit(blk_size);

	/*  When re-using a block, if we always occupy the entire block
		then it's very inefficient.
		It's better to split the block if possible and
		reuse the unneeded part later 	*/


	treshold_size = (*header) >> SPLIT_TRESHOLD_DIVIDER;

			while (blk_size < treshold_size) {
				// split block if possible 
				// Set new size for the unused (untouched) splitted block
				new_size = treshold_size;
				*((size_t *)((char *)header) + blk_size - SIZE_T_SZ) = treshold_size;
				*(size_t *)((char *)header + treshold_size - SIZE_T_SZ) = treshold_size;
				*(size_t *)((char *)header + treshold_size) = treshold_size;
				*((size_t *)header) = treshold_size;

				treshold_size >>= SPLIT_TRESHOLD_DIVIDER;

				if (treshold_size <= MIN_BLK_SZ + 2 * SIZE_T_SZ) {
					*((size_t *)header) = MIN_BLK_SZ + 2 * SIZE_T_SZ;
					break;
				}
			}

	*((size_t *)header) &= 1; //mark as used

	return header; // or ask the OS for more memory	
}



// given pointer to free block header, coalesce with adjacent blocks
// and return pointer to coalesced block (for updating the free nodes linked list)
void *coalesce(size_t *free_blk_hdr_ptr) {

	size_t *next = (size_t *)free_blk_hdr_ptr + (*free_blk_hdr_ptr & ~1L); // next header address
		size_t *prev = (size_t *)free_blk_hdr_ptr -
		(*((size_t *)((char *)free_blk_hdr_ptr - SIZE_T_SZ)) & ~1L);

	//assert ( next!=NULL && prev!=NULL)
	char is_next_allocated = (next < arr + PREALLOCATED_HEAP_SIZE ? *next & 1 : CHAR_MAX);
	char is_prev_allocated = (prev >= arr ? *prev & 1 : CHAR_MAX);

	if (is_prev_allocated && is_next_allocated) {
		return free_blk_hdr_ptr; // can't unite free blocks / heap is a empty
	}
	else if (!(is_prev_allocated) && is_next_allocated) {

	*prev += *free_blk_hdr_ptr; // increase header_sz of prev block
	*(size_t *)((char *)free_blk_hdr_ptr - SIZE_T_SZ) = 0; //clear prev tail 

	// footer/tail will also hold the contain hdr_size
	// in order to invalidate given block and enable us to traverse backwords
	// to the start of combined block
	*(size_t *)((char *)free_blk_hdr_ptr + *free_blk_hdr_ptr - SIZE_T_SZ) = *prev;

	return prev;

	}
	else if (is_prev_allocated && !(is_next_allocated)) {
		// similarly 
		*free_blk_hdr_ptr += *next;
		*(size_t *)((char *)next + *free_blk_hdr_ptr - SIZE_T_SZ) = *free_blk_hdr_ptr;
		return free_blk_hdr_ptr;
	}
	else { //both next and prev are free
		*prev += *free_blk_hdr_ptr + *next;
		*(size_t *)((char *)next + *next - SIZE_T_SZ) = *prev;
		*(size_t *)((char *)free_blk_hdr_ptr + *free_blk_hdr_ptr - SIZE_T_SZ) = 0; //clear input tail
		*(size_t *)((char *)free_blk_hdr_ptr - SIZE_T_SZ) = 0; // clear prev's tail 
		*(size_t *)((char *)next + *next - SIZE_T_SZ) = *prev; // set next's tail
		return prev;
	}

}

/*
	We need to coalesce adjacent free blocks
	have a choice of when to do this:
	1. at search time (find_fit) : deferred coalescing
	2. when freeing: immediate coalescing
*/
void free(void *ptr) {
	size_t *header = (size_t *)((char *)ptr - SIZE_T_SZ);
	*header = *header & ~1L;
	// coalesce if possible
	char* modified_ptr = (char*)(coalesce(header));
	// insert returned pointer into the free blocks linked list ...
}