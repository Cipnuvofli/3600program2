/*
	Written by
	Westley Huebner
	Joseph Penrod
	Tyler Clements
*/

//Define size of memory allowed
#define memsize 10000

struct memBlock {
	int size;
	int status;
	struct memBlock *next;
};
typedef struct memBlock *Mblock;

char mem[memsize];
Mblock Mhead = '\0';
