

//Display function
void nshDisplay(EnvP conductor){

	while (conductor != NULL)
	{
		printf("\t%s = %s\n", conductor->name,conductor->value);//prints the contents of an environment variable in key = value format
		conductor = conductor->next;//Moves on to the next variable
	}
}
//

//Insert function(The comment parameter's description is not accurate, and is an artifact of an earlier scheme). This does work though.
void nshInsert(EnvP *list, char *name, char *value, char *comment){
	int blocksize = sizeof(struct env);
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
	EnvP temp, prev;//Prev is the node before temp in the list
	temp = *list;//The list of variables or aliases being deleted from
	while(temp!=NULL)
	{
		if(strcmp(temp->name,name) == 0)//if the deletion target is found
		{
			if(temp==*list)//if the deletion target is the head of the list
			{
				*list=temp->next;//The head is now the item after the head
				nshFree(temp);//Free the memory allocated to the former head node
			}
			else
			{
				prev->next=temp->next;//Make the node prev connects to the node being checked connects to
				nshFree(temp);//free the memory of the node being checked
			}
		break;//exit the method
		}
		else
		{
			prev=temp;//The previous node is the node being checked
			nshNext(&temp);//Switch nodes
		}
	}

}
//

//Update function
nshUpdate(EnvP *list, char* name, char* value){
	EnvP temp;
	temp = *list;//the list of aliases or environment variables provided as argument
	while(temp!=NULL)
	{
		if(strcmp(temp->name,name) == 0)//If the update target's name is found
		{
			strcpy(temp->value,value);//set the value of the update target to the value argument
			break;//exit the method
		}
		else
		{
			nshNext(&temp);//Go to the next node
		}
	}
}
//

//Find Function
void* nshFind(EnvP list, char* name){
	while(list!=NULL)
	{
		if(strcmp(list->name,name) == 0)//if the target name is found in the list, return a pointer to it
			return list;
		else
			list=list->next;//go to the next node
	}
	return NULL;
}
//


//Returns head of linked list(The way the data structures are organized makes it this simple)
void* nshHead(EnvP item) {
	return item;
}
//


//Go to the next item in linked list
nshNext(EnvP *item){
	(*item) = (*item)->next;//advance the pointer used internally in a method
}
//
