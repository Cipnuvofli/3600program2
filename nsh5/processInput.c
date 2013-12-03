/*
	Written by
	Westley Huebner
	Joseph Penrod
	Tyler Clements

	Last Modified by: Joseph Penrod, 12-2-2013

    Functional Description: This file is devoted to translating the shell's input into the internal commands

*/
#include <stdio.h>
#include "var.h"

void* nshGetExtend(char*);

//Displays, inserts, or updates a new entry
//Input: Aliases or environment variables, the name of a new veriable, and its value
//Output:No direct output. New name and value added to list if successful.
commandSet(EnvP *list, char* name, char* value){
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
				nshInsert(list,name,var->value);
			}
			//If variable doesn't exist, add it
			else
				nshInsert(list,name,value);
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
//Input: Takes the value of the user input.
//Output: Launching the extension process for an environment variable if @ is found.
nshUse(char* value){
	EnvP temp;

	//Check for @ to indicate variable usage
	if(value[0] == '@')
	{

		//Find position of @ character
		char* pos = strchr(value,'@');

		//Check to see if @ is in complex string
		if (nshComplex(value,'@') == 1)
			return;

		//Make sure variable exists
		if (nshFind(var,(pos+1)) != NULL)
		{
			//Find variable that matches
			temp = nshFind(var,(pos+1));
			//value becomes the used variable value
			strcpy(value,temp->value);

		}
		//Check to see if value exists but is being extended (ex. value = @one!@two where one and two exist)
		nshExtend(value);
	}
}


//Extends a variable
//Input: User input with an @sign for an environment variable
//Output: An extended environmnet variable
nshExtend(char* value){

	EnvP temp;
	int i;
	int pos = 0;

	//Create 2 strings to split value and set them to null
	char f[40];
	char s[40];
	memset(&f,'\0',sizeof(f));
	memset(&s,'\0',sizeof(s));

	//Find the position of !
	for(i=0;i<strlen(value);i++)
	{
		//If ! is used and is not in a complex string
		if (value[i] == '!' && nshComplex(value,'!') == 0)
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

	//Check for another extends
	nshExtend(s);

	//Add separator symbol back in
	strcat(f,"!");

	//Combine the values
	strcat(f,s);

	//store the processed value
	strcpy(value,f);

}

//Checks to see if a symbol is found inside of a complex string ({ })
//Input:A complex string input by the user
//Output: Whether or not the symbol is in the brackets
int nshComplex(char* var, char* symb) {
	int j,start,end,check,pos;
	check = 0;
	pos = 0;

	//Check to see if complex string is being used
	if (strchr(var,'{') != NULL && strchr(var,'}') != NULL)
	{
		//Find position of start and end of complex symbols
		for (j=0;j<strlen(var);j++)
		{

			if (var[j] == symb && pos == 0)
				pos = j;
			if (var[j] == '{')
				start = j;
			if (var[j] == '}')
				end = j;
		}
		if (start < pos && end > pos)
			check = 1;

	}

	return check;
}


/*
Checks to see if multiple commands are use (? symbol).

Because this function calls the userInput function, a value is returned
whether or not multiple commands were used. If multiple commands are used,
the calling userInput function will not continue and will instead be
processed here. Removing the check at userInput will run the last command
twice. If multiple commands are not used, then the original userInput
function will continue as normal.

*/
int nshMulti(char* input){

	int i,pos;

	//Checks to see if ? is used outside of a complex string
	if(strchr(input,'?') == NULL || nshComplex(input,'?') == 1)
		//multiple commands were not found
		return 0;

	//Used to hold split input
	char* start;
	char* end;

	//splits input into 2 strings at the first ?
	start = strtok(input,"?");
	end = strtok(NULL,"\0");

	//process the first string
	userInput(start);

	//process the second string
	userInput(end);

	//multiple commands were found
	return 1;
}

//Line extension
//Input: A user input string
//Output: If the $ sign, used for line extension is found, a string with the input before the $sign and where the input is supposed to pick up again, concatenated
void lineExtension(char* input){

	//Check if line extension is used or if it is used outside of a complex string
	if (strchr(input,'$') == NULL || nshComplex(input,'$') == 1)
		return;

	//variable for for loop. Used to find the position of $
	int i;

	//Find where line extension is used ($). Start at the end of the string and work back
	for(i=strlen(input);0<i;i--)
	{
		//If there are spaces or things we don't need, skip them.
		if(input[i] == '\n' || input[i] == ' ')
			continue;
		//When $ is found, exit the loop
		if(input[i] == '$')
			break;
	}

	//Make sure the loop found the $
	if (input[i] == '$')
	{
		//Cut off everything after the $.
		input[i] = '\0';

		//Used to store the extended input
		char extend[80];

		//Get extra input from user
		printf("\t\t-> ");
		fgets(extend,80,stdin);

		//Check for comments
		commentfilter(extend);

		//Combine the old input with the new input
		strcat(input,extend);

		//Check for more line extensions
		lineExtension(input);
	}
}

//Process the echo command

void nshEcho(char* second, char*third){


	//Combine second and third input into 1 string. Add a space betwen the 2 as well.
	strcat(second," ");
	strcat(second,third);

	//Variables for checking individual words.
	char *temp;
	char output[200];

	//get the first word of the input
	strtok(second," ");

	//check to see if there is variable usage
	nshUse(second);

	//Combine the word with the output string and add a space at the end
	strcat(output,second);
	strcat(output," ");

	//get the next word
	temp = strtok(NULL," ");

	//While there are still words to process...
	while(temp != NULL)
	{
		//Check for variable usage
		nshUse(temp);
		//combine the word with the output
		strcat(output,temp);
		//Since variable usage adds a space, check to see if adding a space is necessary
		if(temp[strlen(temp)-1] != ' ')
			strcat(output," ");

		//get the next word
		temp =strtok(NULL," ");
	}
	//Output the processed string
	printf("\t%s\n",output);
}

nshExternal(char *command, char* second, char* third){

	int external,test,check;
	char Path[100] = "/bin";
	EnvP PathBlock;
	char *temp;
	char *temp2;
	char tempPath[100];

	if (nshFind(var,"Path") == NULL)
	{
		printf("\tVariable \"Path\" is not found\n");
		return;
	}

	PathBlock = nshFind(var,"Path");
	strcpy(Path,PathBlock->value);
	//strcat(Path,"/");
	//strcat(Path,command);



	external = fork();
	//external = 0;

	//Child process
	if (external == 0)
	{


		//Split path into 2 tokens
		temp = strtok(Path,"!");
		temp2 = strtok(NULL,"\0");

		//Store the second token to keep it from being modified by strcat
		if(temp2 != NULL)
			strcpy(tempPath,temp2);

		//Loop through each value in the Path variable
		while(temp != NULL)
		{
		//Add command to path value
		strcat(temp,"/");
		strcat(temp,command);

		//Check to see how many arguments are being used
		if (second[0] == '\0')
			execl(temp,command,0,test);
		else if (third[0] == '\0')
			execl(temp,command,second,0,test);
		else
			execl(temp,command,second,third,0,test);


		strcpy(Path,tempPath);
		//Split the path into 2 tokens again
		temp = strtok(Path,"!");
		temp2 = strtok(NULL,"\0");

		//Check to see if temp2 is null
		if(temp2 != NULL)
			strcpy(tempPath,temp2);
		}

		if(!test)
		{
			printf("\t%s %s %s command not found\n",command,second,third);
			_exit(3);
		}

	}
	//parent process
	else
	{
		wait();


	}


}

//Checks for variables that have been extended. This function will separate the values so the function calling
//them can run properly. This is specifically used for multiple paths.

void* nshGetExtend(char* value){

	char *temp = strtok(value,"!");
	secondPath = strtok(NULL,"\0");
	printf("\t%s\t%s\n",temp,secondPath);
	return temp;

}
