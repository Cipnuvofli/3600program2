/*
	Written by:
	Westley Huebner
	Joseph Penrod
	Tyler Clements
*/



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "var.h"

/*
!!!!!!!WARNING!!!!!!!!
setHomeDir does not allow you to debug (not sure why).
If you would like to debug the program, comment out
the contents of setHomeDir. The function does work,
but for some reason it doesn't let the gdb run.
!!!!!!!!!!!!!!!!!!!!!

Returns the home directory.
This code was taken from the Program4 description
*/
//char* setHomeDir(){
//	struct passwd *pw = getpwuid(getuid());
//	char* homedir = pw->pw_dir;
//	return homedir;
//}

//Set native commands
setNative(){
	nshInsert(&native,"set","set");
	nshInsert(&native,"tes","tes");
	nshInsert(&native,"alias","alias");
	nshInsert(&native,"saila","saila");
	nshInsert(&native,"echo","echo");
	nshInsert(&native,"exit","exit");

	//fill alias list for display
	nshInsert(&alias,"set","set");
	nshInsert(&alias,"alias","alias");
	nshInsert(&alias,"tes","tes");
	nshInsert(&alias,"exit","exit");
	nshInsert(&alias,"echo","echo");
	nshInsert(&alias,"saila","saila");

	//Sets the home directory. Comment out when debugging.
//	nshInsert(&var,"Path",setHomeDir());

}

