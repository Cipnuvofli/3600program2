
//Displays, inserts, or updates a new entry
commandSet(EnvP *list, char* name, char* value, char* comment){
	EnvP com,var,temp;
	//make sure var name exists
	if (nshFind(*list,name) != NULL)
		com = nshFind(*list,name);
	else
		com = NULL;

	//Check to see if variable is being extended
	nshExtend(value);

	//Chec to see if variable is being used
	nshUse(value);

	//If a name has been recieved, check to either display or set
	if (name[0] != '\0')
	{
		//If a value has been recieved, set it
		if (value[0] != '\0')
		{
			//If variable already exists, update it
			if (com != NULL)
			{
				nshUpdate(&com,name,value);
				return;
			}

			//If a variable is being used to set value, use the value of the variable
			if (nshFind(*list,value) != NULL)
			{
				var = nshFind(*list,value);
				nshInsert(list,name,var->value, comment);
			}
			//If variable doesn't exist, add it
			else
				nshInsert(list,name,value, comment);
			return;
		}
		//Makes sure variable name exists and displays an appropriate message
		if (com->name == NULL)
			printf("\tInvalid variable or alias\n");
		else
			printf("\t%s = %s\n",com->name,com->value);
		return;
	}
	//If only the command was sent, display the given list
	nshDisplay(*list);
}

//Checks to see when a variable is used
nshUse(char* value){
	EnvP temp;

	//Check for @ to indicate variable usage
	if(strchr(value,'@') != NULL)
	{
		//Find position of @ character
		char* pos = strchr(value,'@');
		//Find variable that matches
		temp = nshFind(var,(pos+1));
		//value becomes the used variable value
		strcpy(value,temp->value);
	}
}


//Extends a variable
nshExtend(char* value){

	EnvP temp;
	int i;
	int pos = 0;

	//Create 2 strings to split value and set them to null
	char f[20];
	char s[20];
	memset(&f,'\0',sizeof(f));
	memset(&s,'\0',sizeof(s));

	//Find the position of !
	for(i=0;i<strlen(value);i++)
	{
		if (value[i] == '!')
		{
			pos=i;
			break;
		}
	}

	//If ! is not there, exit the fuction
	if (pos == 0)
		return;

	//split value into 2 arrays
	for(i=0;i<strlen(value);i++)
	{
		if(pos > i)
			f[i] = value[i];
		if(pos < i)
			s[i-(pos+1)] = value[i];
	}

	//Check for variable use (@)
	nshUse(f);
	nshUse(s);

	//Combine strings
	strcat(f,s);

	//store value as concantinated value
	strcpy(value,f);

}

