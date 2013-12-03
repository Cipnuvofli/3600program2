/*
	Written by
	Westley Huebner
	Joseph Penrod
	Tyler Clements

	Last modified by: Joseph Penrod on 12-2-2013
*/

//Define size of memory allowed
#define memsize 10000

struct memBlock {
	int size;//the size of a memory block
	int status;//whether or not it's in use
	struct memBlock *next;//a pointer to another block
};
typedef struct memBlock *Mblock;//a shorthand for a pointer to a memBlock

char mem[memsize];//the size of the shell's memory
Mblock Mhead = '\0';//The head of the memory array before it's initialized
