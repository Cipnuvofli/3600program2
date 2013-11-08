/*
	Written by Westley Huebner and Joseph Penrod
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "var.h"
#include "mem.h"
#include "sort.h"
#include "set.h"

//While loop check
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

	//Create env var "Path" that stores the home directory
	setHomeDir();
	parseResourceFile("C:\\Users\\Owner\\Desktop\\NSH4\\nshrc.txt");//Parses NSHRC File

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

//handles all user input
userInput(){
	//Clear input fields
	command = NULL;
	memset(&input,'\0',sizeof(input));
	memset(&first,'\0',sizeof(first));
	memset(&second,'\0',sizeof(second));
	memset(&third,'\0',sizeof(third));

	//Take input from user
	printf("nsh4_WesJos$ ");
	fgets(input,80,stdin);


	commentfilter(input);//removes the text after a comment but not inside a complex string from a command.

	//Split the input the user provides
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
		{
			command = nshFind(alias,first);
			handleAlias(command->value);
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
		if ((strcmp(command->value,"exit") == 0) || (strcmp(first,"exit") == 0))
			cont = 0;
	}

}

//Checks to see if an alias has a complex string as a value
handleAlias(char * calias){

	//If alias value leads with complex string symbol, process the function
	if(calias[0] == '{')
	{
		//Removes the leading '{'
		calias = calias + 1;

		//removes the '}'
		strtok(calias,"}");

		//Recombine the input to process again. Add spaces where appropriate
		strcat(calias," ");
		strcat(calias,second);
		strcat(calias," ");
		strcat(calias,third);

		//Process the modified input string
		splitInput(calias);
	}
}

/*
Get the home directory and store it as an env var.
This code was taken from the Program4 description
*/
setHomeDir(){
	struct passwd *pw = getpwuid(getuid());
	char *homedir = pw->pw_dir;
	nshInsert(&var,"Path",homedir);
}

parseResourceFile(char* FILENAME)
{
    FILE *file = fopen(FILENAME, "r");
    if(file!= NULL)
    {
        char line[80];
        while(fgets(line, 80, file)!=NULL)
        {
            commentfilter(line);//removes the text after a comment but not inside a complex string from a command.
            //Split the input the user provides
            splitInput(line);
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
                {
                    command = nshFind(alias,first);
                    handleAlias(command->value);
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
                if ((strcmp(command->value,"exit") == 0) || (strcmp(first,"exit") == 0))
                    cont = 0;
            }
        }
        fclose(file);

    }
    else
    {
        perror("NSHRC File not found");
    }
}
