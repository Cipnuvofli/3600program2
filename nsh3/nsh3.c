#include <stdio.h>
#include <string.h>

#include "var.h"
#include "mem.h"
#include "sort.h"
#include "set.h"

int cont = 1;


main() {

	//fill alias list for display
	nshInsert(&alias,"set","set", '\0');
	nshInsert(&alias,"alias","alias",'\0');
	nshInsert(&alias,"tes","tes",'\0');
	nshInsert(&alias,"exit","exit",'\0');
	nshInsert(&alias,"saila","saila",'\0');

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
	nshInsert(&native,"set","set",'\0');
	nshInsert(&native,"tes","tes",'\0');
	nshInsert(&native,"alias","alias",'\0');
	nshInsert(&native,"saila","saila",'\0');
	nshInsert(&native,"exit","exit",'\0');

}

splitInput(char* input) {
	 //Remove next line character
        strtok(input,"\n");

        //Create pointer to separate input
        char *inputSplit = strtok(input, " !");

        //Make sure there is something for the first argument
        if (inputSplit != NULL)
                strcpy(first,inputSplit);

        //Go to next argument
        inputSplit = strtok(NULL, " !");

        //Make sure there is something for the second argument
        if (inputSplit != NULL)
                strcpy(second, inputSplit);

        //Go to next argument
        inputSplit = strtok(NULL, "\0");

        //Make sure there is something for the third argument
        if (inputSplit !=NULL)
                strcpy(third, inputSplit);


}

commentfilter(char *input)
{
    int lbrace = 0; //if a tilde is found
    int tilde = 0;
    int rbrace = 0;
    int r = 0;//iterates over the input
    for(r =0; r<strlen(input); r++)
    {
        if(input[r] == '{')
        {
            lbrace = r;
        }
        if(input[r] == '~')
        {
            tilde = r;
        }
        if(input[r] == '}')
        {
            rbrace = r;
        }
    }
	//Added a check to see if tilde is there.
    if(lbrace == 0 && rbrace == 0 && tilde != 0)
    {
	//Cut the string off at ~ by replacing it with null. This is essentially what strtok does
        input[tilde] = '\0';
    }
    else if(tilde<lbrace || tilde>rbrace)
    {
	//Same thing here
        input[tilde] = '\0';
    }

}

//handles all user input
userInput(){
	//Clear input fields
	command = NULL;
	memset(&input,'\0',sizeof(input));
	memset(&first,'\0',sizeof(first));
	memset(&second,'\0',sizeof(second));
	memset(&third,'\0',sizeof(third));
	memset(&forth,'\0',sizeof(forth));

	//Take input from user
	printf("nsh3_WesJos$ ");
	fgets(input,80,stdin);


    commentfilter(input);//removes the text after a comment but not inside a complex string from a command.
	splitInput(input);


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
			commandSet(&var,second,third,forth);
		if ((strcmp(command->value,"alias")==0) || (strcmp(first,"alias") == 0))
			commandSet(&alias,second,third, forth);
		if ((strcmp(command->value,"tes") == 0) || (strcmp(first,"tes") == 0))
			nshDelete(&var,second);
		if ((strcmp(command->value,"saila") == 0) || (strcmp(first,"saila") == 0))
			nshDelete(&alias,second);
		if ((strcmp(command->value,"exit") == 0) || (strcmp(first,"exit") == 0))
			cont = 0;
	}

}
