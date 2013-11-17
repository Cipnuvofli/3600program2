/*
	Written by
	Westley Huebner
	Joseph Penrod
	Tyler Clements
*/

#include <stdio.h>
#include <string.h>
#include "var.h"


//While loop check
int cont = 1;


int main(int argc, char *argv[]) {

	//Create native commands
	setNative();

	//Run nshrc.txt if it exists
	parseResourceFile("nshrc.txt",0);

	//Check to see if a file was used as an argument
	if(argv[1]!= NULL)
	{
        	parseResourceFile(argv[1],1);
    	}

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
void userInput(char* input){
	//Clear input fields
	command = NULL;
	memset(&first,'\0',sizeof(first));
	memset(&second,'\0',sizeof(second));
	memset(&third,'\0',sizeof(third));

	//removes the text after a comment but not inside a complex string from a command
	commentfilter(input);

	if(input[0] == '\n' || input[0] == '\0')
	{
		return;
	}


	/*
	Checks if multiple commands are used (? symbol)
	To make sure userInput is called the correct number of times,
	you need to check if the ? symbol was found. If it's there, then
	the function returns 1 and exits out of the userInput function.
	For more details, see nshMulti in processInput.c
	*/
	if(nshMulti(input) == 1)
		return;

	//Split the input the user provides
	splitInput(input);

	if(nshFind(alias,first) !=NULL)
	{
		command = nshFind(alias,first);
		strcpy(first,command->value);

		if(handleAlias(command->value) == 1)
			return;

	}


		//Check command
		if (strcmp(first,"set") == 0)
			commandSet(&var,second,third);
		else if (strcmp(first,"alias") == 0)
			commandSet(&alias,second,third);
		else if (strcmp(first,"tes") == 0)
			nshDelete(&var,second);
		else if (strcmp(first,"saila") == 0)
			nshDelete(&alias,second);
//		else if ((strcmp(command->value,"echo") == 0) || (strcmp(first,"echo") == 0))
//			nshEcho(first, second, third);
		else if (strcmp(first,"exit") == 0)
			cont = 0;
		else
			printf("\tCommand not found\n");

}

//Checks to see if an alias has a complex string as a value
int handleAlias(char * comalias){

	//If alias value leads with complex string symbol, process the function
	if(comalias[0] == '{')
	{

		//Create array to copy passed value to
		char calias2[40];

		//Copy value to array
		strcpy(calias2,comalias);

		//Create pointer for fixed array. This prevents the original value of the parameter from changing
		char *calias = calias2;

		//Removes the leading '{'
		strtok(calias,"{");

		calias = calias + 1;
		//removes the '}'
		strtok(calias,"}");

		//Recombine the input to process again. Add spaces where appropriate

		//Check to see if a second argument was used
		if(second[0] != '\0')
		{
			strcat(calias," ");
			strcat(calias,second);
		}

		//Checl to see if a third argument was used
		if(third[0] != '\0')
		{
			strcat(calias," ");
			strcat(calias,third);
		}

		//Process the modified input string
		userInput(calias);

		//If a complex alias is used, return 1
		return 1;
	}

	//If a complex alias is not used, return 0
	return 0;
}


//This function splits the input stream from the user into different strings for processing.
void splitInput(char* input) {

	 //Remove next line character
	strtok(input,"\n");

        //Create pointer to separate input
        char *inputSplit = strtok(input, " ");

        //Make sure there is something for the first argument
        if (inputSplit != NULL)
	{
                strcpy(first,inputSplit);
	}

        //Go to next argument
        inputSplit = strtok(NULL, " ");

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
void commentfilter(char *input)
{
    int lbrace = 0;
    int tilde = 0;
    //Check to see if tilde is present
    int check = 0;
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
	    check = 1;
        }
        if(input[r] == '}')
        {
            rbrace = r;
        }
    }
	//Added a check to see if tilde is there.
    if(lbrace == 0 && rbrace == 0 && check == 1)
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

/*
Process a file if one is used as an argument when
the program is run or if nshrc.txt exists.
*/
parseResourceFile(char* FILENAME, int argumentfile)
{
    FILE *file = fopen(FILENAME, "r");
    if(file!= NULL)
    {
        char line[80];//same size as the input buffer
        while(fgets(line, 80, file)!=NULL)//reads the lines from the file
        {

		//Process the line retrieved from the file as input
		userInput(line);

		//Reset the line for the next command
		memset(line, '\0', 80);




        }
        fclose(file);//ends the file reading
        if(argumentfile == 1)//if this is one, that means the filename was used as a command line argument and the program should be exited.
        {
            cont = 0;
        }

    }
    else
    {
        perror("NSHRC File not found");
    }
}
