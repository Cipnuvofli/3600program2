#define memsize 3000

struct memBlock {

	int size;
	int status;
	struct memBlock *next;

};
typedef struct memBlock *Mblock;

char mem[memsize];
Mblock Mhead = NULL;
