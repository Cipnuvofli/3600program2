#include "memblock.h"
#include <stdlib.h>


//Malloc function
void* nshMalloc(int s) {

if(sizeof(Mblock)+s > memsize) {
	perror("Out of Memory\n");
	exit(0);
}

if (Mhead == NULL)
{
	Mhead = (Mblock) &mem[0];
	Mhead->size = s;
	Mhead->status = 1;
	Mhead->next = NULL;

	return &mem[sizeof(struct memBlock)];
}
else
{
	Mblock tmp = Mhead;
	int Used;
	int Prev;
	int CurrB;
	int CurrP;

	while(tmp->next != NULL)
	{
		if (tmp->status == 0 && s <=tmp->size)
		{
			break;
		}
		tmp = tmp->next;
	}
	if(tmp->status == 0 && s <= tmp->size)
	{
		CurrB = (long int)tmp-(long int)Mhead;
		CurrP = CurrB + sizeof(struct memBlock);
		tmp->status = 1;
		return &mem[CurrP];
	}

	Prev = (long int)tmp-(long int)Mhead;
	CurrB = Prev + sizeof(struct memBlock)+tmp->size;
	CurrP = CurrB + sizeof(struct memBlock);
	Used = CurrP + s - 1;

	if(Used >= memsize)
	{
		perror("Out of Memory\n");
		error(0);
	}

	Mblock newB = (Mblock) &mem[CurrB];
	newB->size = s;
	newB->status = 1;
	newB->next = NULL;
	tmp->next = newB;
	return &mem[CurrP];

}

}

void nshFree(void *item) {

	Mblock temp = Mhead;
	while (temp+1 != (Mblock)item)
		temp = temp->next;
	temp->status = 0;

}

void* nshCalloc(int s) {
	int *temp =nshMalloc(s);
	int i;
	if (temp != NULL)
	{
		for(i=0;i<s;i++)
		{
			//mem[i+sizeof(struct memBlock)] = 0;
			*(temp+1) = 0;
		}
	}
	return (void*)temp;


}

void* nshRealloc(void* ptr, int s) {

	if(ptr == NULL)
		return nshMalloc(s);

	nshFree(ptr);
	return nshMalloc(s); 


}
