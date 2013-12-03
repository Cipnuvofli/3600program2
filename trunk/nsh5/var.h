/*
	Written by:
	Westley Huebner
	Joseph Penrod
	Tyler Clements

	Last Modified by: Joseph Penrod on 12-2-2013
*/

/*
The Env data structure stores a name or alias in the 100 character array that is name
The Value array stores what the name is supposed to be equal to
the next point takes the program to another env


The EnvP is shorthand for a pointer to an Env

*/

struct env {
	char name[100];
	char value[200];
	struct env *next;
};

typedef struct env *EnvP;

/*
	var is used for environmental variables
	alias is used for alias commands
	native is used for commands that cannot be removed
*/
EnvP var,alias,native;

//input strings
EnvP command;//Stores the input command once it's identity is determined
char input[80];//Stores user input
char first[20];//Stores the first token
char second[120];//Stores the second token
char third[120];//Stores the thrid token
char *secondPath;//Used for handling multidirectory versions of the path environment variable
