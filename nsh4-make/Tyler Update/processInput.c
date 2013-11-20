/*
	Written by Westley Huebner and Joseph Penrod
*/
#include <stdio.h>
#include "var.h"

//Displays, inserts, or updates a new entry
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

void nshEcho(char* first, char* second, char* third) {

	int i,j,k,l,m;

	int boolThird[2] = {0,0};
	int boolFourth[2] = {0,0};
	int boolFifth[2] = {0,0};

	char* token;

	char fullString[120];

	//These are needed to make the preexisting functions
	//work (i.e. nshUse, etc.). I made 5 parts just to 
	//be sure that the input won't break.
	char tempFirst[20];
	char tempSecond[20];
	char tempThird[20];
	char tempFourth[20];
	char tempFifth[20];

	//Combining inputs for better processing, also
	//temporarily partitioning in the process
	for(i=0; i < strlen(first); i++) {
		fullString[i] = first[i];
		tempFirst[i] = first[i];
	}

	fullString[i] = ' ';
	i++;

	for(j=0; j < strlen(second); j++) {
		fullString[i] = second[j];
		tempSecond[j] = second[j];
		i++;
	}

	fullString[i] = ' ';
	i++;

	l=0;
	m=0;
	for(k=0; k < strlen(third); k++) {

		fullString[i] = third[k];
		
		//This is partitioning the third input into separate
		//strings based on the space character. This will
		//allow the premade functions to work properly. The
		//boolThird[1], for example, is a flag saying if it's
		//been used, skip it, so it won't be overwritten. The
		//boolThird[0], for example, is a flag saying if it's
		//the active partition being written to (only one
		//partition should be active at a time with this 
		//checking method).
		if(third[k] == ' ') {
			if(boolThird[1] == 0) {
				boolThird[0] = 1;
			}

			m++;
		}
		if(boolThird[0] == 1) {
			if( (third[k] == ' ') && (m == 2) ) {
				boolThird[0] = 0;
				boolThird[1] = 1;
				l = 0;

				boolFourth[0] = 1;
			}

			else {
				tempThird[l] = third[k];
				l++;
			}
		}
		if(boolFourth[0] == 1) {
			if( (third[k] == ' ') && (m == 3) ) {
				boolFourth[0] = 0;
				boolFourth[1] = 1;
				l = 0;

				boolFifth[0] = 1;
			}
			else {
				tempFourth[l] = third[k];
				l++;
			}
		}
		if(boolFifth[0] == 1) {
			if( (third[k] == ' ') && (m == 4) ) {
				boolFifth[0] = 0;
				boolFifth[1] = 1;
				l = 0;

				printf("Error. Line Length has been exceeded.\n");
				break;
			}
			else {
				tempFifth[l] = third[k];
				l++;
			}
		}

		i++;
	}

	//Iterate through input for symbols
	for(i=0; i< strlen(tempSecond); i++) {

		//Searching for @ symbol in second string
		if(tempSecond[i] == '@') {
			nshUse(tempSecond);
			printf("%s\n",tempSecond );
		}
		//Searching for ? symbol in second string
		if(tempSecond[i] == '?') {
			nshMulti(tempSecond);
		}
		//Searching for $ symbol in second string
		if(tempSecond[i] == '$') {
			nshLineEx(tempSecond);
		}
		
	}
	for(i=0; i< strlen(tempThird); i++) {

		//Searching for @ symbol in second string
		if(tempSecond[i] == '@') {
			nshUse(tempThird);
			printf("%s\n",tempThird);
		}
		//Searching for ? symbol in second string
		if(tempThird[i] == '?') {
			nshMulti(tempThird);
		}
		//Searching for $ symbol in second string
		if(tempThird[i] == '$') {
			nshLineEx(tempThird);
		}
		
	}
	for(i=0; i< strlen(tempFourth); i++) {

		//Searching for @ symbol in second string
		if(tempFourth[i] == '@') {
			nshUse(tempFourth);
			printf("%s\n",tempFourth );
		}
		//Searching for ? symbol in second string
		if(tempFourth[i] == '?') {
			nshMulti(tempFourth);
		}
		//Searching for $ symbol in second string
		if(tempFourth[i] == '$') {
			nshLineEx(tempFourth);
		}
		
	}
	for(i=0; i< strlen(tempFifth); i++) {

		//Searching for @ symbol in second string
		if(tempFifth[i] == '@') {
			nshUse(tempFifth);
			printf("%s\n",tempFifth );
		}
		//Searching for ? symbol in second string
		if(tempFifth[i] == '?') {
			nshMulti(tempFifth);
		}
		//Searching for $ symbol in second string
		if(tempFifth[i] == '$') {
			nshLineEx(tempFifth);
		}
		
	}
}

void nshLineEx(char* lineOne) {
	
	//Just a counter variable
	int i;

	//Search first partition for '$'. Shouldn't be one here,
	//but it's searching just in case.(We all know how
	//users are)
	for(i=0; i < strlen(lineOne); i++) {

		if(lineOne[i] == '$') {
			lineOne[i] = '\0';
			printf("\t\t");
			fgets(second,20,stdin);
			printf("%s %s\n", lineOne, second);
		}
	}

/*
	//Searching second partition for '$'
	for(i=0; i < strlen(second); i++) {

		if(second[i] == '$') {
			second[i] = '\0';
			printf("\t\t");
			fgets(third,40,stdin);
			printf("\n%s %s", second, third);
		}
	}
	//Searching third partition for '$'
	for(i=0; i < strlen(third); i++) {
	
		if(third[i] == '$') {
			third[i] = '\0';
			printf("\t\t");
			fgets(third,40,stdin);
			printf("\n%s %s", second, third);
		}
	}
*/

}