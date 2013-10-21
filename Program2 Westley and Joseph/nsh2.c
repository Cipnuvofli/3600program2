#include <stdio.h>
#include <string.h>

#include "var.h"
#include "mem.h"
#include "sort.h"
#include "set.h"

int cont = 1;

main() {

	//fill alias list for display
	nshInsert(&alias,"set","set");
	nshInsert(&alias,"alias","alias");
	nshInsert(&alias,"tes","tes");
	nshInsert(&alias,"exit","exit");
	nshInsert(&alias,"saila","saila");

	//Create native commands
	setNative();

	//Arguably an uneeded loop, but it keeps the program running.
	while(cont)
	{
		userInput();
	}


}

//Set native commands
setNative(){
	nshInsert(&native,"set","set");
	nshInsert(&native,"tes","tes");
	nshInsert(&native,"alias","alias");
	nshInsert(&native,"saila","saila");
	nshInsert(&native,"exit","exit");

}

//handles all user input
userInput(){
	//Clear input fields
	command = NULL;
	memset(&input,'\0',sizeof(input));
	memset(&first,'\0',sizeof(first));
	memset(&second,'\0',sizeof(second));
	memset(&third,'\0',sizeof(third));

	//Take input from user
	printf("nsh1_WesJos$ ");
	fgets(input,60,stdin);
	sscanf(input,"%s %s %s",first,second,third);

	//Make sure command or alias exists
	if ((nshFind(alias,first) == NULL) && (nshFind(native,first) == NULL))
	{
		printf("\tCommand not found.\n");
		userInput();
	}
	else
	{
		//Checks if alias or native command passed fail test
		if (nshFind(alias,first) == NULL)
			command = nshFind(native,first);
		else
			command = nshFind(alias,first);

		//Check command
		if ((strcmp(command->value,"set")==0) || (strcmp(first,"set") == 0))
			commandSet(&var,second,third);
		if ((strcmp(command->value,"alias")==0) || (strcmp(first,"alias") == 0))
			commandSet(&alias,second,third);
		if ((strcmp(command->value,"tes") == 0) || (strcmp(first,"tes") == 0))
			nshDelete(&var,second);
		if ((strcmp(command->value,"saila") == 0) || (strcmp(first,"saila") == 0))
			nshDelete(&alias,second);
		if ((strcmp(command->value,"exit") == 0) || (strcmp(first,"exit") == 0))
			cont = 0;
	}

}
