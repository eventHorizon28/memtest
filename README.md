# memtest
# Assign1


Prerequisites
We wanted to  implement malloc() and free() library calls for dynamic memory allocation that detect common programming and usage errors.

Breakdown

-Malloc returns a void pointer and free sets the array which  acts as the heap
-We need to Indicate the size allocated to the user and wether the block is free or not.

-We decided to store the metadata as bits instead of  an implicit linked list to make better use of space/time

-This was done through bit masking

-To combat fragmentation we need to coalasce
-In order to coalasce the free blocks ,We checked the block infront of it wether its free or not
to coalasce the free block behind we iterated through the argument keeving a prev in order to return to the previous free block

-We needed to in a data-structure to simulate the heap so we are given a 5000 byte char array.

- size (of type int ) + indicator (unused or unallocated for this node, of type int or short)


Running the tests




Responding to Detected Errors
Invalid pointer/Null/out of bounds-->
x=error message
printf("x at line %d, in file %s", line_no, file_name);
 
Built With:

vim-editor used
