/*
	Written by Westley Huebner and Joseph Penrod
*/

#include "memblock.h"
#include <stdio.h>

//Malloc function
void* nshMalloc(int s) {

//Makes sure new block doesn't exceed the total size of memory allowed
if(sizeof(Mblock)+s > memsize) {
	perror("Out of Memory\n");
	return NULL;
}

//Creates the first entry in memory
if (Mhead == NULL)
{
	Mhead = (Mblock) &mem[0];
	Mhead->size = s;
	Mhead->status = 1;
	Mhead->next = NULL;

	return &mem[sizeof(struct memBlock)];
}
//Process memory
else
{
	//Initialize variables
	Mblock temp = Mhead;
	int Used;
	int Prev;
	int CurrB;
	int CurrP;

	//Find next spot in memory linked list to insert new memory block
	while(temp->next != NULL)
	{
		if (temp->status == 0 && s <=temp->size)
		{
			break;
		}
		temp = temp->next;
	}

	//Checks if previously freed memory is available to use
	if(temp->status == 0 && s <= temp->size)
	{
		CurrB = (long int)temp-(long int)Mhead;
		CurrP = CurrB + sizeof(struct memBlock);
		temp->status = 1;
		return &mem[CurrP];
	}

	//Variables used to find the position in the memory array to use
	Prev = (long int)temp-(long int)Mhead;
	CurrB = Prev + sizeof(struct memBlock)+temp->size;
	CurrP = CurrB + sizeof(struct memBlock);

	//Checks to see if new block will fit
	Used = CurrP + s - 1;

	if(Used >= memsize)
	{
		perror("Out of Memory\n");
		//error(0);
		return NULL;
	}


	//Add new memory block to end of linked list
	Mblock newB = (Mblock) &mem[CurrB];
	newB->size = s;
	newB->status = 1;
	newB->next = NULL;
	temp->next = newB;
	return &mem[CurrP];

}
}

//Free method
void nshFree(void *item) {

	//Finds location of memory to remove
	Mblock temp = Mhead;
	while (temp+1 != (Mblock)item)
		temp = temp->next;

	//Lets Malloc know it can use this memory block
	temp->status = 0;
}

//Calloc method
void* nshCalloc(int s) {

	//Allocate memory
	int *temp =nshMalloc(s);
	int i;

	//Checks to see if it's the first entry
	if (temp != NULL)
	{
		//Set memory equal to 0
		for(i=0;i<s;i++)
		{
			*(temp+1) = 0;
		}
	}
	return (void*)temp;
}

//Realloc method
void* nshRealloc(void* ptr, int s) {

	//Create block to copy existing block
	Mblock temp =(Mblock)ptr;
	int i;

	//If there's nothing in the memory linked list, just malloc
	if(temp == NULL)
		return nshMalloc(s);

	//Create new entry to put at end of linked list
	Mblock  newB = nshMalloc(s);

	//Copy information from old block to new block
	for(i=1;i <= s; i++)
	{
		newB[i] = temp[i];
	}

	//Fix size
	newB->size = s;

	//Remove old block
	nshFree(ptr);

	//Return new location
	return newB;


}
