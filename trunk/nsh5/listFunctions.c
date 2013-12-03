/*
	Written by:
	Westley Huebner
	Joseph Penrod
	Tyler Clements

    Last Modified by: Joseph Penrod, 12-2-2013

    Functional Description: This file is devoted to working with the data structure that stores Environment Variables/Aliases.


    1. NSH Display displays the contents of the structure conductor points too
    2. NSH Insert adds an environment variable or alias to a list.
    3. NSH Delete removes an item from a list.
    4. NSH Update overwrites the value of an existing environment variable or alias
    5. NSH Find returns the list that was used as an argument if the name argument exists as a name in the list
    6. HSH Head takes a pointer to the head of the list and returns the same pointer. It seems like a redundancy.
    7. NSH Next advances a pointer's location in its corresponding data structure.
*/
#include <stdio.h>
#include "var.h"

//Display function
void nshDisplay(EnvP conductor){

	while (conductor != NULL)
	{
		printf("\t%s = %s\n", conductor->name,conductor->value);
		conductor = conductor->next;
	}
}
//

//Insert function
void nshInsert(EnvP *list, char *name, char *value){

	EnvP temp = (EnvP) nshMalloc(sizeof(struct env));
	if (temp == NULL)
	{
		return;
	}
	EnvP left,right;
	right = *list;
	int found = 0;
	int i;
	left = NULL;
	strcpy(temp->name,name);
	strcpy(temp->value,value);

	//Checks to see if list is empty
	if (right == NULL)
	{
		*list=temp;
		(*list)->next = NULL;
		return;
	}

	//If list is not empty, insert new item alphabetically
	while(right != NULL && found == 0)
	{
		for (i = 0; i < sizeof(right->name);i++)
		{
			//If letter matches,keep checking current var
			if (right->name[i] == name[i])
			{
				found = 1;
				continue;
			}
			//if new var is after current var, keep searching
			else if (right->name[i] < name[i])
			{
				left=right;
				nshNext(&right);
				break;
			}
			//if new var is before current var
			else if (right->name[i] > name[i])
			{
				found = 1;
				break;
			}

		}


	}

	//Store new value
	//Checks if new value will be first in list
	if (left == NULL)
	{
		temp->next = *list;
		*list = temp;
	}
	else
	{
		left->next = temp;
		temp->next = right;
	}
}
//

//Delete Function
void nshDelete(EnvP *list, char* name){
	EnvP temp, prev;
	temp = *list;
	while(temp!=NULL)
	{
		if(strcmp(temp->name,name) == 0)
		{
			if(temp==*list)
			{
				*list=temp->next;

				nshFree(temp);
			}
			else
			{
				prev->next=temp->next;
				nshFree(temp);
			}
		break;
		}
		else
		{
			prev=temp;
			nshNext(&temp);
		}
	}

}
//

//Update function
nshUpdate(EnvP *list, char* name, char* value){
	EnvP temp;
	temp = *list;
	while(temp!=NULL)
	{
		if(strcmp(temp->name,name) == 0)
		{
			strcpy(temp->value,value);
			break;
		}
		else
		{
			nshNext(&temp);
		}
	}
}
//

//Find Function
void* nshFind(EnvP list, char* name){
	while(list!=NULL)
	{
		if(strcmp(list->name,name) == 0)
			return list;
		else
			list=list->next;
	}
	return NULL;
}
//


//Returns head of linked list
void* nshHead(EnvP item) {
	return item;
}
//


//Go to the next item in linked list
nshNext(EnvP *item){
	(*item) = (*item)->next;
}
//
