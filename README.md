# memtest
# Assign1


Prerequisites
We wanted to  implement malloc() and free() library calls for dynamic memory allocation that detect common programming and usage errors.

Breakdown

-We need to Indicate the size allocated to the user and wether the block is free or not.

-Malloc returns a void pointer and free sets the array which  acts as the heap

-In order to coalasce the free blocks, we iterated through the argument keeving a prev in order to return to the previous free block

-We needed to store the strings in a data-structure since the compartments were unknown.

- size (of type int or short ) + indicator (unused or unallocated for this node, of type int or short)




Built With:

vim-editor used




Contributors/Authors

Kamarudeen Ayankunbi

Kshitij Bafna

Acknowledgments

Professor John-Austen
