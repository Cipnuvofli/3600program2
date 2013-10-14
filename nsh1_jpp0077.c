
/*
    CSCE 3600
    NSH1
    Author: Joseph Penrod
    Note: This program works much better in the Codeblocks debugger than it does anywhere else. I have no idea why this is the case, or any more time to figure it out barring an extension.
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<strings.h>
#include<sys/types.h>
#include<unistd.h>
#include<memory.h>
#define COMMAND_SIZE 64


typedef struct aliasStorage
{
    char *Alias;
    char *Value;

    struct aliasStorage *next;
}ALIAS;


typedef struct envStorage
{
    char *envVariable;
    char *Value;

    struct envStorage *next;
}ENV;

struct block {
	int size;
	int status;
	struct block *next;
};
typedef struct block *BlockP;

BlockP MemHead = NULL;
ENV *Environmentvariables;
int position = 0;
ALIAS *Aliases;
char **Tokens;//The +1 is for checking if the 3rd argument is NULL, it doesn't work if you don't allocate it.
char memory[1048576];//Memory For implementation in Part 2
int q;//For Loop Iterator

//Counts and returns the number of tokens in a command
int argumentCounter(char *command)
{

    int counter = 0;
    int emptystringflag = 1;
    for(counter = 0; counter<COMMAND_SIZE; counter++)
    {
        if(command[counter] != ' ')
        {
            emptystringflag = 0;
        }
    }
    if(emptystringflag == 1)
    {
        return 0;
    }
    counter = 1;
    while((command = strpbrk(command, " !"))!=NULL)
    {
        counter++;
        command++;
    }
    return counter;
}
//Displays all environment variables and corresponding values
void Envdisplay(ENV *evlist)
{
    q = 0;
    while(q<16)
    {
        if(evlist[q].envVariable!=NULL)
        {
                printf("%s=%s\n\n", evlist[q].envVariable, evlist[q].Value);
        }
        q++;
    }
}
//Finds a specific Environment Variable, and displays it with its corresponding value
void TargetedEnvdisplay(ENV *evlist, char *target)
{
    q = 0;
    while(q<16)
    {
        if(evlist[q].envVariable!= NULL)
        {
            if(strcmp(target, evlist[q].envVariable) == 0)
            {
                printf("%s=%s\n\n", evlist[q].envVariable, evlist[q].Value);
                return;
            }
        }
        q++;


    }
        printf("Variable not found\n");
}
//Adds an environment variable to where they're stored
void Envpush(ENV *evlist, char *Variable, char *Val)
{
    q=0;
    if(evlist[0].envVariable == NULL)
    {

        evlist[0].envVariable = (char*)nshCalloc(strlen(Variable), memory, &position);
        evlist[0].Value = (char*)nshCalloc(strlen(Val), memory, &position);
        strcpy(evlist[0].envVariable, Variable);
        strcpy(evlist[0].Value, Val);
    }
    else
    {
      while(evlist[q].envVariable!=NULL)
      {
          if(strcmp(evlist[q].envVariable, Variable) == 0)
          {
              evlist[q].Value = (char*)nshCalloc(strlen(Val), memory, &position);
              strcpy(evlist[q].Value, Val);
              return;
          }
          q++;
      }
      if(q<16)
      {
        evlist[q].envVariable = (char*)nshCalloc(strlen(Variable), memory, &position);
        evlist[q].Value = (char*)nshCalloc(strlen(Val), memory, &position);
        strcpy(evlist[q].envVariable, Variable);
        strcpy(evlist[q].Value, Val);
      }
      else
      {
          printf("Support for arbitrary numbers of environment variables is being worked on\n");
      }
    }
}

void Envextend(ENV *evlist, char* extended, char* extension)
{
    for(q = 0; q<16; q++)
    {
        if(evlist[q].Value!= NULL)
        {
                 if(strcmp(extended, evlist[q].Value) == 0 || strcmp(extension, evlist[q].Value) == 0)
                {
                    if(sizeof(extended)+sizeof(extension)<COMMAND_SIZE)
                    {
                        strcpy(evlist[q].Value, strcat(extended,extension));
                        return;
                    }
                }
        }

    }
}
//Nullifies an environment variable if present
void Envdelete(ENV *evlist, char *target)
{
    for(q = 0; q<16; q++)
    {
        if(evlist[q].envVariable!= NULL)
        {
            if(strcmp(target, evlist[q].envVariable) == 0)
            {
            evlist[q].envVariable = NULL;
            evlist[q].Value = NULL;
            return;
            }
        }

    }
        printf("Variable not found\n");
}
char* envReplace(ENV *evlist, char **Target)//finds an environment variable and replaces it with its value in evlist
{
    char *OverflowSafety = (char*)nshCalloc(strlen(*Target), memory, position);
    strcpy(OverflowSafety, *Target+strlen(*Target)+1);
    if (*Target[0] == '@')
    {
        memmove(*Target, *Target+1, strlen(*Target));
        for(q = 0; q<16; q++)
        {
            if(evlist[q].envVariable!=NULL)
            {
                if(strcmp(*Target, evlist[q].envVariable)==0)
                {
                    strcpy(*Target, evlist[q].Value);
                    return OverflowSafety;
                }
            }
        }
                printf("Variable not found\n");
    }
    else
    {
        printf("A value that didn't start with @ was an argument\n");
       nshFree(OverflowSafety);
        return NULL;
    }
}
char* aliasReplace(ALIAS *Aliaslist, char **Target)
{
    char *OverflowSafety = (char*)nshCalloc(strlen(*Target), memory, &position);
    strcpy(OverflowSafety, *Target+strlen(*Target)+1);
    for(q = 0; q<16; q++)
    {
        if(Aliaslist[q].Alias!=NULL)
        {
            if(strcmp(*Target, Aliaslist[q].Alias)==0)
            {
                strcpy(*Target, Aliaslist[q].Value);
                return OverflowSafety;
            }
        }
    }
            printf("Variable not found\n");
}
//Displays all Aliases and values
void Aliasdisplay(ALIAS *Aliaslist)
{
    q = 0;
    while(q<16)
    {
        if(Aliaslist[q].Alias!=NULL)
        {
            printf("%s=%s\n\n", Aliaslist[q].Alias, Aliaslist[q].Value);
        }
        q++;
    }

}
//Displays an Alias and its value
void TargetedAliasdisplay(ALIAS *Aliaslist, char *target)
{
    q = 0;
    while(q<16)
    {
        if(Aliaslist[q].Alias!= NULL)
        {
            if(strcmp(target, Aliaslist[q].Alias) == 0)
            {
                printf("%s=%s\n\n", Aliaslist[q].Alias, Aliaslist[q].Value);
                return;
            }
        }
        q++;
    }
        printf("Variable not found\n\n");
}

void Aliaspush(ALIAS *Aliaslist, char *Variable, char *Val)
{

    q=0;
    if(Aliaslist[0].Alias == NULL)
    {

        Aliaslist[0].Alias = (char*)nshCalloc(strlen(Variable), memory, &position);
        Aliaslist[0].Value = (char*)nshCalloc(strlen(Val), memory, &position);
        strcpy(Aliaslist[0].Alias, Variable);
        strcpy(Aliaslist[0].Value, Val);
    }
    else
    {
      while(Aliaslist[q].Alias!=NULL)
      {
          if(strcmp(Aliaslist[q].Alias, Variable) == 0)
          {
              Aliaslist[q].Value = (char*)nshCalloc(strlen(Val), memory, &position);
              strcpy(Aliaslist[q].Value, Val);
              return;
          }
          q++;
      }
      if(q<16)
      {
        Aliaslist[q].Alias = (char*)nshCalloc(strlen(Variable), memory, &position);
        Aliaslist[q].Value = (char*)nshCalloc(strlen(Val), memory, &position);
        strcpy(Aliaslist[q].Alias, Variable);
        strcpy(Aliaslist[q].Value, Val);
      }
      else
      {
          printf("Support for arbitrary numbers of environment variables is being worked on\n");
      }
    }
}
//Finds and nullifies an alias and value if present
void Aliasdelete(ALIAS *Aliaslist, char *target)
{

    for(q = 0; q<16; q++)
    {

        if(Aliaslist[q].Alias!=NULL)
        {

            if(strcmp(target, Aliaslist[q].Alias) == 0)
            {
                Aliaslist[q].Alias = NULL;
                Aliaslist[q].Value = NULL;
                return;
            }
        }
    }
        printf("Alias not found\n");
}
void commandParser(char *command)
{
    int argCount = argumentCounter(command);
    int i;
    //Begin String Tokenizer
    Tokens = (char**)nshCalloc(argCount+1*sizeof(char*), memory, &position);//The +1 is for checking if the 3rd argument is NULL, it doesn't work if you don't allocate it.
    for(i = 0; i<argCount; i++)
    {
        Tokens[i] = (char*)nshCalloc(COMMAND_SIZE, memory, &position);//This part may go wrong for some reason outside the debugger. I have no idea why yet
    }
    for(i = 0; i<argCount; i++)
    {
        if (i ==0)
        {
           Tokens[i] = strtok(command, " !");
        }
        else
        {
            Tokens[i] = strtok(NULL, " !");
        }
    }
    //End String Tokenizer
    if(argCount == 0)
    {

    }
    else if(strcmp(Tokens[0], "set") == 0||strcmp(Tokens[0], "set\n")==0)
    {
        if(argCount == 1)
        {
            Envdisplay(Environmentvariables);
        }
        else if(Tokens[1]!= NULL && Tokens[2]== NULL && argCount ==2)
        {
            TargetedEnvdisplay(Environmentvariables, Tokens[1]);
        }
        else if(Tokens[1]!=NULL && Tokens[2]!=NULL&& argCount == 3)
        {
            Envpush(Environmentvariables, Tokens[1], Tokens[2]);
        }
        else if(Tokens[3]!=NULL&&argCount == 4)
        {
            if(Tokens[2][0]=='@')//Adding content onto the end of an envrionment variable
            {
                Envextend(Environmentvariables, Tokens[2], envReplace(Environmentvariables, &Tokens[2]));
            }
            else if(Tokens[3][0] == '@')//Appending the Environment Variable to the end
            {
                envReplace(Environmentvariables, &Tokens[3]);
                Envextend(Environmentvariables, Tokens[2],Tokens[3]);
            }
            else
            {
                printf("No recognized commands\n");
            }
        }

    }
    else if(strcmp(Tokens[0], "tes") == 0||strcmp(Tokens[0], "tes\n") == 0)
    {
        if(Tokens[1]!=NULL)
        {
            Envdelete(Environmentvariables, Tokens[1]);
        }
    }
    else if(strcmp(Tokens[0], "alias") == 0||strcmp(Tokens[0], "alias\n") == 0)
    {
        if(argCount == 1)
        {
            Aliasdisplay(Aliases);
        }
        else if(Tokens[1]!=NULL && Tokens[2]==NULL)
        {
            TargetedAliasdisplay(Aliases, Tokens[1]);
        }
        else if(Tokens[1]!=NULL && Tokens[2]!=NULL)
        {
            Aliaspush(Aliases, Tokens[1], Tokens[2]);
        }

    }
    else if(strcmp(Tokens[0], "saila") == 0||strcmp(Tokens[0], "saila\n"  ) == 0)
    {
        if(Tokens[1]!=NULL && argCount == 2)
        {
            Aliasdelete(Aliases, Tokens[1]);
        }
        else if(argCount == 1)
        {
            printf("Too few arguments\n");
        }
        else
        {
            printf("Too many arguments\n");
        }
    }
    else if(strcmp(Tokens[0], "exit")== 0||strcmp(Tokens[0], "exit\n")== 0)
    {

    }
    else
    {
        printf("argCount = %d\n", argCount);
		printf("External Command, displaying only.\n");
		for(i=0; i<argCount; i++)
		{
		printf("Argument %d is %s\n", i, Tokens[i]);
		}

    }
    for(i = 0; i<argCount; i++)
    {
        if(strcmp(Tokens[i], "exit")!= 0)
        {
               nshFree(Tokens[i]);
        }
    }
    nshFree(Tokens);

}


int main(int argc, char *argv[])
{
   if(argc > 1)
    {
        printf("Called with command line arguments\n");
        for(q = 0; q<argc; q++)
        {
            printf("%s\n", argv[q]);
        }
        return 0;
    }
    Environmentvariables = (ENV*)nshCalloc(2*COMMAND_SIZE+4, memory, &position);
    Aliases = (ALIAS*)nshCalloc(2*COMMAND_SIZE+4, memory, &position);
    memset(memory, 0, 1048576);//Clears the memory of Garbage Data
    Envpush(Environmentvariables, "Home", "Directory");//Eventually this will actually be a directory path
    char command[COMMAND_SIZE];//Command size currently limited to 64 characters including null terminator

    while(strcmp("exit", command)!= 0)
	{
		memset(command, 0, sizeof(command));//zeroes command
		printf("nsh: ");//prompt
		fgets(command, COMMAND_SIZE, stdin);//receives input
		printf("\n");
		command[strlen(command)-1] = '\0';
		commandParser(command);

	}

    return 0;
}
