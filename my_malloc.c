#include<stdio.h>
#include<stdlib.h>
#include "my_malloc.h"
#define CODE_BITS 262143
#define ALL_BITS 2147483648
//#define FIRST_BIT 2147483648
#define SIZE_BITS 8191
#define CODE 209715
#define FREE 0
#define USED 1

char myblock[5000];

/*void printBinary(int num, int num_bits)
{
	int i;
	unsigned int masker = (unsigned int)1<<num_bits;
	for(i = 0; i<32; i++)
	{
		if(((unsigned int)num & masker)==0) 
			printf("0 ");
		else
			printf("1 ");
		masker>>=1;
		//printf("n = %d\n", n);
	}

}*/
int verified(int num)
{
	if(((unsigned int)num & CODE_BITS) == CODE)
	{
		//printf("verified\n");
		return 1;
	}

	//printf("fail\n");
	return 0;
}
int isUsed(int num)
{
	//printf("isUsed = %u\n", ((unsigned int)num>>31));
	return ((unsigned int)num>>31);
}
int getSize(int num)
{
	//printf("size = %u\n", ((unsigned int)num>>18)&SIZE_BITS);
	return (((unsigned int)num >> 18))&SIZE_BITS;
}

int getMeta(int size, int used)
{
	//used bit
	int i = used;
	i = i<<13;
	//size bits: 13
	i = i|size;

	i <<= 18;
	//code bits:11001100110011
	i |= 209715;

	return i;
}

void printFree()
{
	int* i = (int*)myblock;
	int free_space = 0;

	while((void*)i<(void*)(myblock+5000))
	{
		if(verified(*i))
		{
			if(!isUsed(*i))
			{
				free_space += getSize(*i);
			}
			i = (void*)i+getSize(*i)+4;
		}
	}
	printf("total free space = %d\n", free_space);
}

void* my_malloc(int num_bytes, char* file_name, int line_no)
{
	int i;
	char* head_ptr = myblock;
	unsigned int x = *((int*)head_ptr);
	void* ret_ptr = head_ptr;
	int * meta_ptr;

	printf("malloc-ing %d bytes\n", num_bytes);
	if(num_bytes>5000-4)
	{
		printf("Not enough space to allocate memory at line %d, in file %s\n", line_no, file_name);
		return 0;
	}
	//verify if it is not the first malloc
	if(verified(x))
	{
	//if it is not->

		while(isUsed(x) && ret_ptr<=(void*)(myblock+5000))
		{
			ret_ptr = ret_ptr + getSize(x) + 4;
			x = *((int*)ret_ptr);
			
			if(ret_ptr>(void*)(myblock+5000)-4)
			{
				printf("Not enough space to allocate memory at line %d, in file %s\n", line_no, file_name);
				return 0;
			}

			if(!isUsed(x))
			{
				if(num_bytes>getSize(x)-4)
				{
					ret_ptr = ret_ptr + getSize(x)+4;
					x=*((int*)ret_ptr);
				}
			}
		}

		if(ret_ptr>(void*)(myblock+5000)-4)
		{
			printf("Not enough space to allocate memory at line %d, in file %s\n", line_no, file_name);
			return 0;
		}

		//set metadata of the allocated size
		meta_ptr = (int*)ret_ptr;

		//get metadata of the new size of the free block
		//here '-4' will be only once for the free metadata
		//since the newly malloced space's metadata is being stored
		//in the block where previously free's metadata was
		x = getMeta(getSize(x)-num_bytes-4, FREE);
		
		//get and set metadata for the used block
		*meta_ptr = getMeta(num_bytes, USED);

		//go to starting of the remaining free space
		//set metadata of the remaining free space
		meta_ptr = (int*)((void*)meta_ptr+num_bytes+4);
		*meta_ptr = x;
		
		//return pointer to the starting of the allocated memory
		return (ret_ptr+4);
	}
	else
	{
		//set metadata of the allocated space
		x = getMeta(num_bytes, USED);
		meta_ptr = (int*)ret_ptr;
		*meta_ptr = x;
		
		//metadata of the remaining free space
		//5000 - num_bytes - metadata for the space allocated - metadata for the free space
		x = getMeta(5000-num_bytes-4-4, FREE);

		//go to starting of the remaining free space
		//set metadata of the remaining free space
		meta_ptr = (int*)((void*)meta_ptr+num_bytes+4);
		*meta_ptr = x;
		
		return (ret_ptr +4);
	}
}

void my_free(void* free_ptr,char* file_name, int line_no)
{
	
	int curr_meta_int;
	int* curr_meta_ptr;
	int* next_meta_ptr;
	int* prev_meta_ptr;

	printf("%s\n", file_name);

	if(free_ptr == NULL)
	{
		printf("NULL pointer exception at line %d, in file %s\n", line_no, file_name);
	}
	if(free_ptr<(void*)myblock || free_ptr>(void*)(myblock+5000))
	{
		printf("out of bounds exception at line %d, in file %s\n", line_no, file_name);
		return;
	}

	curr_meta_ptr = free_ptr-4;
	curr_meta_int = *curr_meta_ptr;

	if(!verified(curr_meta_int))
	{
		printf("trying to free an invalid pointer at line %d, in file %s", line_no, file_name);
		return;
	}
	else
	{
		if(!isUsed(*curr_meta_ptr))
			return;
	}

	//if its all good!
	//go forward to check if that block free
	next_meta_ptr = (void*)curr_meta_ptr+getSize(curr_meta_int)+4;
	if((void*)next_meta_ptr < (void*)(myblock+5000))
	{
		if(!isUsed(*next_meta_ptr) && verified(*next_meta_ptr))
		{
		
			curr_meta_int = getMeta(getSize(curr_meta_int)+getSize(*next_meta_ptr)+4, FREE);
			*curr_meta_ptr = curr_meta_int;
			//setting the merged free's metadata to be 0 so that we don't verify it as our metadata in the future
			*next_meta_ptr = 0;
			//printf("sizeof merged free block = %d\n", getSize(curr_meta_int));
		}
	}

	//merge the prev block if it is free
	//for that, iterate thru the myblock and find a free block right before the block being freed.
	prev_meta_ptr = (int*)myblock;
	
	//increment the prev_meta_ptr until it reaches the block right before curr_meta_ptr
	if(prev_meta_ptr != curr_meta_ptr)
	{
		while(((void*)prev_meta_ptr+getSize(*prev_meta_ptr)+4) != curr_meta_ptr)
		{
			prev_meta_ptr = (void*)prev_meta_ptr+getSize(*prev_meta_ptr)+4;
		}	

		//if it is free merge it with the curr_meta_ptr
		if(!isUsed(*prev_meta_ptr) && verified(*prev_meta_ptr))
		{
			*prev_meta_ptr = getMeta(getSize(*curr_meta_ptr)+getSize(*prev_meta_ptr)+4, FREE);
			*curr_meta_ptr = 0;
		}
	}


	//if curr_meta_ptr is the only free, then just change the isUsed bit to 0
	if(verified(*curr_meta_ptr))
	{
		*curr_meta_ptr = getMeta(getSize(*curr_meta_ptr), FREE);
	}
}

/*int main()
{
	int i = 0;
	char * ptr;
	for(i = 0; i<150; i++)
	{
		ptr = (char*)my_malloc(1, "malloc2", 34);
		printFree();
		my_free(ptr, "freeptr1", 195);
		printFree();
		my_free(ptr, "free again", 197);
	}

	return 0;
}
*/
