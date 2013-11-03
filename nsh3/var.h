struct env {
	char name[100];//variable name
	char value[100];//what the variable equals
	struct env *next;//a link to the next envvariable
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
char first[20];//these arrays are for tokens
char second[20];
char third[20];
char forth[20];
