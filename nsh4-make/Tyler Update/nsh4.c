/*
	Written by Westley Huebner and Joseph Penrod
*/

#include <stdio.h>
#include <string.h>
#include "var.h"


//While loop check
int cont = 1;


int main() {

	//Create native commands
	setNative();

	//Continues while cont = 1. To close the program, set cont to 0.
	while(cont)
	{
		memset(&input,'\0',sizeof(input));
		printf("nsh4_WesJosTyl$ ");
		fgets(input,80,stdin);
		userInput(input);
	}


}


//handles all user input
userInput(char* input){
	//Clear input fields
	command = NULL;
	memset(&first,'\0',sizeof(first));
	memset(&second,'\0',sizeof(second));
	memset(&third,'\0',sizeof(third));




	/*
	Checks if multiple commands are used (? symbol)
	To make sure userInput is called the correct number of times,
	you need to check if the ? symbol was found. If it's there, then
	the function returns 1 and exits out of the userInput function.
	For more details, see nshMulti in processInput.c
	*/
	if(nshMulti(input) == 1)
		return;

	commentfilter(input);//removes the text after a comment but not inside a complex string from a command.

	//Split the input the user provides
	splitInput(input);



	//Make sure command or alias exists
	if ((nshFind(alias,first) == NULL) && (nshFind(native,first) == NULL))
	{
		printf("\tCommand not found.\n");
		return;
	}
	else
	{
		//Checks if alias or native command passed fail test
		if (nshFind(alias,first) == NULL)
			command = nshFind(native,first);
		else
		{
			command = nshFind(alias,first);
			if (handleAlias(command->value) == 1)
				return;
//			handleAlias(command->value);
		}
		//Check command
		if ((strcmp(command->value,"set")==0) || (strcmp(first,"set") == 0))
			commandSet(&var,second,third);
		if ((strcmp(command->value,"alias")==0) || (strcmp(first,"alias") == 0))
			commandSet(&alias,second,third);
		if ((strcmp(command->value,"tes") == 0) || (strcmp(first,"tes") == 0))
			nshDelete(&var,second);
		if ((strcmp(command->value,"saila") == 0) || (strcmp(first,"saila") == 0))
			nshDelete(&alias,second);
		if ((strcmp(command->value,"echo") == 0) || (strcmp(first,"echo") == 0))
			nshEcho(first, second, third);
		if ((strcmp(command->value,"exit") == 0) || (strcmp(first,"exit") == 0))
			cont = 0;
	}

}

//Checks to see if an alias has a complex string as a value
handleAlias(char * comalias){

	//If alias value leads with complex string symbol, process the function
	if(comalias[0] == '{')
	{

		char* calias = comalias;
		//Removes the leading '{'
		calias = calias + 1;

		//removes the '}'
		strtok(calias,"}");

		//Recombine the input to process again. Add spaces where appropriate
		strcat(calias," ");
		strcat(calias,second);
		strcat(calias," ");
		strcat(calias,third);

		userInput(calias);
		return 1;
		//Process the modified input string
//		splitInput(calias);
	}
	return 0;
}


//This function splits the input stream from the user into different strings for processing.
splitInput(char* input) {
	 //Remove next line character
	strtok(input,"\n");

        //Create pointer to separate input
        char *inputSplit = strtok(input, " !");

        //Make sure there is something for the first argument
        if (inputSplit != NULL)
	{
                strcpy(first,inputSplit);
	}

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

//This function nullifies anything following the ~ symbol in the input string.
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
	//Checking again to make sure tilde is used
    else if((tilde<lbrace || tilde>rbrace) && tilde != 0)
    {
	//Cut string off at ~
        input[tilde] = '\0';
    }

}

