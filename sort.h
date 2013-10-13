//Display function
void nshDisplay(EnvP conductor){

	while (conductor != NULL)
	{
		printf("%s = %s\n", conductor->name,conductor->value);
		conductor = conductor->next;
	}
}
//

//Insert function
void nshInsert(EnvP *list, char *name, char *value){
//!!!!!!Malloc!!!!!!!!
	EnvP temp = (EnvP)malloc(sizeof(EnvP));
	EnvP left,right;
	right = *list;
	int found = 0;
	int i;

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
				break;
			}
			//if new var is before current var
			else if (right->name[i] > name[i])
			{
				found = 1;
				break;
			}

		}
		//Go to next item in list
		left=right;
		right=right->next;
	}
	//Store new value;
	left->next=temp;
	left=temp;
	left->next = right;
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
//!!!!!!FREE!!!!!!!!
				free(temp);
			}
			else
			{
				prev->next=temp->next;
//!!!!!!!FREE!!!!!!!!
				free(temp);
			}
		break;
		}
		else
		{
			prev=temp;
			temp=temp->next;
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
			temp = temp->next;
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
}
//
