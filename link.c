#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "sort.h"

main(){
	//strcpy(head->name,"first");
	//strcpy(head->value,"1");
	//head->name = 10;
	//head->next= 0;
	nshInsert(&head,"First","22efefefefefe");
	nshInsert(&head,"Second","4e;fjkoeapfhe1");
	nshInsert(&head,"Third","eihfelkjehfjkeb");
	nshInsert(&head,"tird","ee;nelkfne");
	//nshDelete(&head,"First");
	//nshUpdate(&alias,"Second","3");
	
	//EnvP command = nshFind(head,"First");
	//printf("%s\n",command->value);
	nshDisplay(head);
}
