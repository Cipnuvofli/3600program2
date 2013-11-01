#include <stdio.h>
#include <string.h>

#include "var.h"
#include "mem.h"
#include "sort.h"
#include "set.h"

int cont = 1;
 int r = 0;//Nullifies stuff after one iteration of comment

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

char * commentfilter(char *input)
{
    strtok(input, "~");
	char *comment;
	comment = strtok(NULL, "~");


	for(r = 0; r<strlen(first); r++)
    {
        if(first[r] == '~')
        {
            memset(&first[r], '\0', strlen(first)-r);
            memset(&second[r], '\0', strlen(second)-r);
            memset(&third[r], '\0', strlen(third)-r);
            memset(&forth[r], '\0', strlen(forth)-r);
        }
    }
    for(r = 0; r<strlen(second); r++)
    {
         if(second[r] == '~')
        {
            memset(&second[r], '\0', strlen(second)-r);
            memset(&third[r], '\0', strlen(third)-r);
            memset(&forth[r], '\0', strlen(forth)-r);
        }
    }
     for(r = 0; r<strlen(third); r++)
    {
         if(third[r] == '~')
        {
             memset(&third[r], '\0', strlen(third)-r);
        }
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


	splitInput(input);
	commentfilter(input);
	printf("%s", input);

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
