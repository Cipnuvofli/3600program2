#ifndef VAR_H
#define VAR_H

struct env {
	char name[100];
	char value[100];

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
char input[60];
char first[20];
char second[20];
char third[20];

#endif // VAR_H
