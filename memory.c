#include<stdio.h>
#include<memory.h>
//test
void *nshMalloc(int bytes, char Memory[], int *position)
{
        int magnitude = 64;
        int start = *position;
        void *retval = NULL;
        while (magnitude<bytes)
        {
            magnitude*=2;
            if(magnitude >2046)//Leave some degree of wiggle room
            {
                printf("You are exceeding the limits of the memory management system\n.");
                break;
            }
        }

                *position+=(magnitude);//moves position the length of the block
                Memory[*position] = '\0';//terminates the block
                *position = *position+1;//moves the position value for a new block
                retval = &Memory[start];
                return retval; //starting postion of the allocated block of memory

}

void*  nshCalloc(int bytes, char Memory[], int *position)//with the exception of a for loop, this is almost exactly like mschMalloc
{
        int magnitude = 64;
        int start = *position;
        int q = 0; //used for for loops
        char *retval = NULL;
        while (magnitude<bytes)
        {
            magnitude*=2;
            if(magnitude >2046)//Leave some degree of wiggle room
            {
                printf("You are exceeding the limits of the memory management system\n.");
                break;
            }
        }

            *position+=(magnitude);//moves position the length of the block
            for(q = start; q<*position; q++)
            {
                Memory[q] = '\0';//zeroes the block
            }
            Memory[*position] = '\0';
            *position=*position+1;//moves the position value for a new block
           retval = &Memory[start];
           return retval; //starting postion of the allocated block of memory
}

void nshRealloc(char memory[], int bytes, char *memPtr)//Copies the block into a larger one, deletes old block
{

}

void nshFree(char *memptr)//Zeroes a block?
{
    while(*memptr != '\0')
    {
        *memptr = '\0';
        memptr++;
    }
}


