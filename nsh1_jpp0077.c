
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
#define COMMAND_SIZE 64


typedef struct aliasStorage
{
    char *Alias;
    char *Value;
}ALIAS;


typedef struct envStorage
{
    char *envVariable;
    char *Value;
}ENV;

ENV *Environmentvariables;
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

        evlist[0].envVariable = calloc(1, strlen(Variable));
        evlist[0].Value = calloc(1, strlen(Val));
        strcpy(evlist[0].envVariable, Variable);
        strcpy(evlist[0].Value, Val);
    }
    else
    {
      while(evlist[q].envVariable!=NULL)
      {
          if(strcmp(evlist[q].envVariable, Variable) == 0)
          {
              evlist[q].Value = calloc(1 ,sizeof(Val));
              strcpy(evlist[q].Value, Val);
              return;
          }
          q++;
      }
      if(q<16)
      {
        evlist[q].envVariable = calloc(1, sizeof(Variable));
        evlist[q].Value = calloc(1, sizeof(Val));
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
                        realloc(evlist[q].Value, strlen(extended)+strlen(extension));
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
void envReplace(ENV *evlist, char *Target)//finds an environment variable and replaces it with its value in evlist
{
    if (Target[0] == '@')
    {
        memmove(Target, Target+1, strlen(Target));
        for(q = 0; q<16; q++)
        {
            if(evlist[q].envVariable!=NULL)
            {
                if(strcmp(Target, evlist[q].envVariable)==0)
                {
                    strcpy(Target, evlist[q].Value);
                    return;
                }
            }
        }
                printf("Variable not found\n");
    }
    else
    {
        printf("A value that didn't start with @ was an argument\n");
        return;
    }



}
void aliasReplace(ALIAS *Aliaslist, char *Target)
{

    for(q = 0; q<16; q++)
    {
        if(Aliaslist[q].Alias!=NULL)
        {
            if(strcmp(Target, Aliaslist[q].Alias)==0)
            {
                strcpy(Target, Aliaslist[q].Value);
                return;
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

        Aliaslist[0].Alias = calloc(1, strlen(Variable));
        Aliaslist[0].Value = calloc(1, strlen(Val));
        strcpy(Aliaslist[0].Alias, Variable);
        strcpy(Aliaslist[0].Value, Val);
    }
    else
    {
      while(Aliaslist[q].Alias!=NULL)
      {
          if(strcmp(Aliaslist[q].Alias, Variable) == 0)
          {
              Aliaslist[q].Value = calloc(1 ,sizeof(Val));
              strcpy(Aliaslist[q].Value, Val);
              return;
          }
          q++;
      }
      if(q<16)
      {
        Aliaslist[q].Alias = calloc(1, sizeof(Variable));
        Aliaslist[q].Value = calloc(1, sizeof(Val));
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
    Tokens = calloc(argCount+1, sizeof(char*));//The +1 is for checking if the 3rd argument is NULL, it doesn't work if you don't allocate it.
    for(i = 0; i<argCount; i++)
    {
        Tokens[i] = (char*)calloc(COMMAND_SIZE, sizeof(char));//This part may go wrong for some reason outside the debugger. I have no idea why yet
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
                envReplace(Environmentvariables, Tokens[2]);
                Envextend(Environmentvariables, Tokens[2],Tokens[3]);
            }
            else if(Tokens[3][0] == '@')//Appending the Environment Variable to the end
            {
                envReplace(Environmentvariables, Tokens[3]);
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
        free(Tokens[i]);
    }
    free(Tokens);

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
    Environmentvariables = calloc(16, 2*COMMAND_SIZE+4);
    Aliases = calloc(16, 2*COMMAND_SIZE+4);
    Envpush(Environmentvariables, "Home", "Directory");//Eventually this will actually be a directory path
    memset(memory, 0, 1048576);//Clears the memory of Garbage Data
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
