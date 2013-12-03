/*
	Written by:
	Westley Huebner
	Joseph Penrod
	Tyler Clements
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
EnvP command;
char input[80];
char first[20];
char second[120];
char third[120];
char *secondPath;
