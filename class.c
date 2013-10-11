//Meta Node for malloc. Always going to be 12 bytes.
//header.h
struct block {
	int size;
	int status;
	struct block *next;
};
typedef struct block *BlockP;

char mem[SIZEMEM];
BlockP headMeta = Null;
//

//List Node
//list.h
struct node {
	char item[size];
	char value[size];
	struct node *next;
}
typedef node *NodeP;
//

//Main file
#include "header.h"
#include "list.h"

int main(int arc, char *argv){
NodeP headSet = NULL;
NodeP headAlias = NULL;

nshInsert (&headset, "v1", "11");
nshInsert (&headset, "v2", "22");
nshRemove (&headset, "v2");
nshDisplay (headSet);
dispMem();
printf("\n");

nshInsert (&headset, "v3", "33");
nshDisplay (headSet);
dispMem();
printf("\n");

void *tmpP = (NodeP) nshFind (&headSet, "v3");
void *newTmpP = (NodeP) nshRealloc (tmpP, 36);
printf("find v3 at: %p\n", tmpP);
printf("newTmpP: %p\n", newTmpP);
dispMem();
return 0;
}
//

//memory functions
void* handleMem(int s) {
if (sizeof (struct block)+s > SIZEMEM) {
	perror ("Out of memory\n");
	exit(0);
}
headMeta = (BlockP) &mem[0];
headMeta->size = s;
headMeta->status = 1;
headMeta->next = NULL;

return &mem[sizeof(struct block)];
}
else {
BlockP tmp = headMeta;
int idxMemUsed;
int idsPrevBlck;
int indCurrBlck
int idxCurrPtr;

while (tmp->next !=NULL) {
if tmp=>status ==0 && s <= tmp->size) {
	break;
}
tmp = tmp->next;
}
if (tmp->status ==0 && s<= tmp->size) {
	idxCurrBlck = (long int)tmp-(long int)headMeta;
	idxCurrPtr = idxCurrBlck + sizeof (struct block);
	tmp->status =1;
	return &mem[idxCurrPtr];
}
udxPRevBlck = (long int)tmp-(long int)headMeta;
idxCurrBlck = idxPrevBlck + sizeof (struct block)+tmp->size;
idxCurrPtr = idxCurrBlck + sizeof (strict block);
idxMemUsed = idxCurrPtr + s - 1;

if (idxMemUsed>= SIZEMEM) {
	perror("Out of memroy");
	error(0);
}

BlockP newB = (BlockP) &mem[idxCurrBlck];
newB->size = s;
newB->status = 1;
newB->next = NULL;

return &mem[idxCurrPTr];
}
}

//Malloc that I can see
void *mshMalloc (int s) {
return handleMem(s);
}
//


//more methods
//Display
void nshDisplay (NodeP hd) {
NodeP tmp = hd;
int i=0;
printf("list");
while (tmp != NULL) {
	printf(tmp, tmp->item, tmp->value, tmp->next);
	tmp = tmp->next;
}
}

//Insert
void nshInsert (NodeP *hd, char *item, char *value) {
NodeP aNodeP = (NodeP)nshMalloc (sizeof (struct node));

}
