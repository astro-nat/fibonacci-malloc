/* 
    File: my_allocator.h

    Author: Derek Wene
            Department of Computer Science
            Texas A&M University
    Date  : 02/13/2014


*/
#include <stdlib.h>
#include <stdio.h>
#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 

typedef struct
{
	int MAGIC;
	Addr previous;
	Addr next;
	int length;
	int inh;
	int lr;
	bool is_free;

}HDR;

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */


/*--------------------------------------------------------------------------*/
/* MODULE   MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

unsigned int init_allocator(unsigned int _basic_block_size, 
			    unsigned int _length); 
/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. 
*/ 

int release_allocator(); 
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 

Addr my_malloc(unsigned int _length); 
/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 

int my_free(Addr _a); 
/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 
   
 
//I need these helper functions. Please include in H file 
int nearest_fib(int size); // Returns the nearest fibonacci number to the argument size
int index_fib(int fib); //Returns the index in the freelist to a perticular fibonacci number
int index_number_blocks(int index); // Returns the fibonacci number for a index in the freelist
void divide_memory (int index); // Does the actual memory division
void add_to_freelist(HDR* p1); // adds a memory chunk to the freelist
void remove_from_list(HDR* node); // after merging two chunks of memory, the right chunk is removed from the freelist
int printfreelist(); // uses printf to print the freelist in a visual format
int printmemoryinorder(); // prints the memory headers in order
int nearest_fib_upper(int size); // returns the nearest fibonacci, rounded up
int checkfreelist(HDR* pointer); // used for testing purposes


#endif
