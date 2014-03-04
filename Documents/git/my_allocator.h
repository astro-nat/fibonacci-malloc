/* 
    File: my_allocator.h

    Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

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
int nearest_fib(int size);
int index_fib(int fib);
int index_number_blocks(int index);
void divide_memory (int index);
void add_to_freelist(HDR* p1);
void remove_from_list(HDR* node);
HDR* find_buddy(HDR* header);
int printfreelist();
int printmemoryinorder();
int nearest_fib_upper(int size);
void check_size(int size);
int checkfreelist(HDR* pointer);


#endif