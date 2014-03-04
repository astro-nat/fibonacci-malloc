/* 
    File: my_allocator.c

    Author: <Derek Wene>
            Department of Computer Science
            Texas A&M University
    Date  : <2/12/2014>


    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "my_allocator.h"
#include <string.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

int BASIC_BLOCK_SIZE = 0;
int NUMBER_BLOCKS = 0;
int HIGHEST_INDEX = 0;
HDR* freelist[50];
Addr first_node;
HDR* max_mem;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */
unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length)
{
	BASIC_BLOCK_SIZE = _basic_block_size;
	int usable_length = _length - sizeof(HDR);
	NUMBER_BLOCKS= (usable_length)/_basic_block_size;
	HIGHEST_INDEX = index_fib(nearest_fib(NUMBER_BLOCKS));
	HIGHEST_INDEX = index_fib(nearest_fib(NUMBER_BLOCKS));
	Addr start = malloc(NUMBER_BLOCKS * _basic_block_size);
	HDR* begin = (HDR*) start;
	first_node = begin;
	begin->lr = NULL;
	begin->next = NULL;
	begin->previous=NULL;
	begin->is_free=1;
	begin->inh = NULL;
	begin->MAGIC=993;
	begin->length=index_number_blocks(HIGHEST_INDEX)* _basic_block_size;

	freelist[HIGHEST_INDEX] = begin;
	max_mem = (HDR*)((unsigned int)begin + begin->length);
	return NUMBER_BLOCKS * _basic_block_size;
}
int release_allocator()
{
	free(first_node);
}


extern Addr my_malloc(unsigned int _length) {

	int needed_bytes =(int) (((size_t)_length) + ((size_t)sizeof(HDR)))/BASIC_BLOCK_SIZE;	
  	int requested_number_blocks = nearest_fib_upper(needed_bytes+1);//This finds number of blocks requested
  	int index = index_fib(requested_number_blocks);
  	int i=index;

	while(index<=HIGHEST_INDEX && i<=HIGHEST_INDEX)
  	{
  		if(freelist[index] != NULL)
  		{
		HDR* node = freelist[index];
			while(node !=NULL)
			{

				if(node->is_free ==1)
				{
				    node->is_free=0;
				    if(node->MAGIC != 993)
				    {
					    printf("UH OH! Magic number not right in my_malloc");
					    HDR* pointer_kill =(HDR*) 123;
					    pointer_kill->next = NULL;
				    }
				    return (Addr)(((size_t)node)+((size_t)sizeof(HDR)));
				    
				}
				node = (HDR*)node->next;
			}
  		}

  		if(i <=HIGHEST_INDEX)//make sure we dont go out of range.
  		{
// 		    		
				HDR* node = freelist[i];
				while(node !=NULL)
				{
					if(node->is_free==1)
					{
						divide_memory(i);
						i=index-1;
						break;
					}
					node =(HDR*) node->next;
				}
  		}
  		i++;
  	}
  	
  	
 	printf("OUT OF MEMORY, CRASHING\n");
	return 0;
}

extern int my_free(Addr _a) 
{
  
	HDR* freed =(HDR*) (((size_t)_a) -((size_t)sizeof(HDR)));
	HDR* freed_buddy;
	if(freed->MAGIC !=993)
	{
		printf("Magic number incorrect in my_free");
		freed =(HDR*) 123;
	}
	freed->is_free=1;
	
	while(true)
	{
	
		if(freed->lr == 0 && index_fib(freed->length)!=0)//if its left and buddy is ready
		{
			freed_buddy =(HDR*) (((size_t)freed) + ((size_t)freed->length));
			if((index_fib(freed_buddy->length) == (index_fib(freed->length)-1)) && (freed_buddy->is_free== 1))
			{
				remove_from_list(freed);
				remove_from_list(freed_buddy);
				HDR* left = freed;
				int new_inh=freed_buddy->inh;
				int new_lr = freed->inh;
				left->is_free=1;
				left->length = freed->length + freed_buddy->length;
				left->MAGIC=993;
				left->lr = new_lr;
				left->inh = new_inh;
				left->next=NULL;
				left->previous=NULL;
				add_to_freelist(left);
				freed = left;
			}
			else
			{
			  break;
			}
		}
		else if(freed->lr == 0 && index_fib(freed->length)==0)
		{
			freed_buddy =(HDR*) (((size_t)freed) + ((size_t)freed->length));
			if((index_fib(freed_buddy->length) == (index_fib(freed->length))) && (freed_buddy->is_free== 1))
			{
				remove_from_list(freed);
				remove_from_list(freed_buddy);
				HDR* left = freed;
				int new_inh=freed_buddy->inh;
				int new_lr = freed->inh;
				left->is_free=1;
				left->length = freed->length + freed_buddy->length;
				left->MAGIC=993;
				left->lr = new_lr;
				left->inh = new_inh;
				left->next=NULL;
				left->previous=NULL;
				add_to_freelist(left);
				freed = left;
				printf("new node is at address: 0x%08x\n",freed );
			}
			else
			{
			  break;
			}
		}  
		else if(freed->lr == 1)
		{
			if(index_fib(freed->length/BASIC_BLOCK_SIZE) != 0)
			{
				int freed_buddy_length = index_number_blocks(index_fib(freed->length/BASIC_BLOCK_SIZE) + 1)*BASIC_BLOCK_SIZE;
				freed_buddy =(HDR*) (((size_t)freed) - freed_buddy_length);
				if((freed_buddy->length == freed_buddy_length) && (freed_buddy->is_free == 1))
				{
				      remove_from_list(freed);
				      remove_from_list(freed_buddy);
				      HDR* left = freed_buddy;
				      int new_inh=freed->inh;
				      int new_lr = freed_buddy->inh;
				      left->is_free=1;
				      left->length = freed->length + freed_buddy->length;
				      left->MAGIC=993;
				      left->lr = new_lr;
				      left->inh = new_inh;
				      left->next=NULL;
				      left->previous=NULL;
				      add_to_freelist(left);
				      freed = left;
				}
				else
				{
				      break;
				}
			}	
			else
			{
				int freed_buddy_length = index_number_blocks(1)*BASIC_BLOCK_SIZE;
				freed_buddy = (HDR*)(((size_t)freed) - freed_buddy_length);
				if(freed_buddy->length != index_number_blocks(1)*BASIC_BLOCK_SIZE)
				{
				      freed_buddy_length =  index_number_blocks(0)*BASIC_BLOCK_SIZE;
				      freed_buddy = (HDR*)(((size_t)freed) - freed_buddy_length);
				}
				else{}
				if(freed_buddy->is_free)
				{
				      remove_from_list(freed);
				      remove_from_list(freed_buddy);
				      HDR* left = freed_buddy;
				      int new_inh=freed->inh;
				      int new_lr = freed_buddy->inh;
				      left->is_free=1;
				      left->length = freed->length + freed_buddy->length;
				      left->MAGIC=993;
				      left->lr = new_lr;
				      left->inh = new_inh;
				      left->next=NULL;
				      left->previous=NULL;
				      add_to_freelist(left);
				      freed = left;
				}
				else
				{
					break;
				}
			}
		}
		else
		{ 
			break;
		}
	}
	freed->is_free=1;
	return 0;
}


int nearest_fib(int size)//returns nearest fibinachi number
{
	int a = 2;
	int b=1;
	while(size>a)
	{
		a=a+b;
		b=a-b;
	}
	return b;
}
int nearest_fib_upper(int size)
{
	int a = 2;
	int b=1;
	while(size>a)
	{
		a=a+b;
		b=a-b;
	}
	return a;  
}
int index_fib(int fib)//Gives index from fib number
{
  if(fib==1)
    return 0;
	int a, b;
	a=2;
	b=1;
	int counter=1;
	while(fib>a)
	{
		a=a+b;
		b=a-b;
		counter++;
	}
	return counter;
}
int index_number_blocks(int index)//Gives me fib number from index
{
	int a, b;
	  
	a=2;
	b=1;
	for(int i=1; i<=index; i++)
	{
		a=a+b;
		b=a-b;
	}
	return b;	
}

void divide_memory(int index)//TODO: what if its not free. Need to deal with this
{	
	if(index>0 && index<=HIGHEST_INDEX)
	{
		HDR* temp = freelist[index];
		while(temp!=NULL)
		{
			if(temp->is_free==1)
			{
				if(freelist[index] == temp)
				{
					freelist[index] = (HDR*)freelist[index]->next;
					if(freelist[index] !=NULL)
						freelist[index]->previous = NULL;
				}
				else
				{
					HDR* previous =(HDR*)temp->previous;
					HDR* next = (HDR*)temp->next;
					if(next !=NULL)
						next->previous =(Addr) previous;
					if(previous !=NULL)
						previous->next =(Addr) next;
				}
				int orig_inh = temp->inh;
				int orig_lr = temp->lr;
				int p1_size = index_number_blocks(index-1);

				int p2_size = index_number_blocks(index-1);
				if(index!=1)
					p2_size = index_number_blocks(index-2);
				HDR* p1=temp;
				int p2_number_bytes =(size_t) p1_size*BASIC_BLOCK_SIZE;
				
				int p2_location =(size_t) p1+ p2_number_bytes;
				HDR* p2 =(HDR*) p2_location;
				p1->lr=0;
				p1->inh = orig_lr;
				p2->inh = orig_inh;
				p2->lr=1;
				p1->length=p1_size*BASIC_BLOCK_SIZE;
				p2->length=p2_size*BASIC_BLOCK_SIZE;
				p1->next = p2-> next = p1-> previous = p2-> previous = NULL;
				p1->is_free=p2->is_free=1;
				p1->MAGIC=p2->MAGIC=993;
				add_to_freelist(p1);
				add_to_freelist(p2);
			}
			temp=(HDR*)temp->next;	
		}
	}
}

void add_to_freelist(HDR* p1)
{
	int index = index_fib(p1->length/BASIC_BLOCK_SIZE);

	HDR* temp = freelist[index];
	if(freelist[index]!=NULL)
 	{	

		freelist[index]->previous=(Addr)p1;
		p1->next=(Addr)freelist[index];
		freelist[index]= p1;
	}
	else
	{
		freelist[index] = p1;
	}
}

void remove_from_list(HDR* node)
{

	int index = index_fib(node->length/BASIC_BLOCK_SIZE);

	HDR* hdr = freelist[index];
	while(hdr!=NULL)
	{
		if(hdr==node)
		{

			HDR* previous =(HDR*) hdr->previous;
			HDR* next = (HDR*) hdr->next;
			if(previous!=NULL)
			    previous->next = (Addr) next;
			if(next!=NULL)
			    next->previous = (Addr) previous;
			hdr->next=NULL;
			hdr->previous = NULL;
			if(freelist[index] ==node)
				freelist[index] = (HDR*)next;
			return;
		}
		else
			hdr = (HDR*)hdr->next;
	}
	
// 	printf("I removed nothing, I'm dumb\n");
	HDR* kill = (HDR*) 123;
	kill->is_free = 2;
}
int checkfreelist(HDR* pointer)
{
  int find= -1;
  HDR* node;
	for(int i =0; i<=HIGHEST_INDEX; i++)
	{
		  node = freelist[i];
		  while(node!=NULL)
		  {
			 if(pointer == node)
			 {
				return i;
			 }
			 node =(HDR*) node->next;
		  }
	}
  return find;
}
