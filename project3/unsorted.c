/*
 * Evan Chou Project 3 Unsorted
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"set.h"

//structure for set
typedef struct set
{
	int count; //number of strings held in the string array or set
	int length; //length of array of character pointers and max number of elements allowed
	char **data; //pointing to array of character pointers that each point to a string
}SET;

//private function search prototype
static int search(SET *,char *);

//function for creating set
SET *createSet(int maxElts)
{
	SET *sp; //pointing to structure SET
	sp=malloc(sizeof(SET));
	assert(sp!=NULL);
   	sp->count=0; //initialize 
	sp->length=maxElts; //max number elements given as maxElts
	sp->data=malloc(sizeof(char*)*maxElts); //allocate memory for array to strings	
	assert(sp->data!=NULL);
	return sp;
}

//function for deleting set data
void destroySet(SET *sp)
{
	//checks if set is empty
	assert(sp!=NULL);

	//free pointer for each string in character pointer array to delete all strings
	for(int i=0;i<sp->count;i++)
	{
		free(sp->data[i]);
	}
	free(sp->data);
	free(sp);
	return;
}

//function will return number of elements in the set
int numElements(SET *sp)
{
	return sp->count;
}

//function will input new string into set
void addElement(SET *sp,char *elt)
{   
  	//checks if the element number already equals to the array maximum, then cannot add
	assert(sp->count<sp->length);

	//checks if the new elements is unique
	if(search(sp,elt)!=-1)
	{
		return;
	}
	sp->data[sp->count]=strdup(elt); //string duplicate to copy string and return pointer 
	sp->count++;
	return;
}

//function will remove given string from set
void removeElement(SET *sp,char *elt)
{
	//checks if set is empty
	assert(sp!=NULL);

	//search for index of specific string
	int index=search(sp,elt);

	//check to see if specific string exists
	if(index==-1)
	{
		return;
	}
	sp->count--;	
	sp->data[index]=sp->data[sp->count];
	return;
}

//function will return specific string 
char *findElement(SET *sp,char *elt)
{
	//search for index of specific string
	int index=search(sp,elt);

	//checks to see if specific string exists
	if(index==-1)
	{
		return NULL;
	}
	return sp->data[index]; 
}

//function will return identical set pointer
char **getElements(SET *sp)
{
	char **temp=malloc(sizeof(char*)*sp->count);
	assert(temp!=NULL);

	//copies memory for new array of character pointers
	memcpy(temp,sp->data,sizeof(char*)*sp->count); 
	return temp;
}

//searches for specific element
static int search(SET *sp, char *elt)
{
	int i;
	for(i=0;i<sp->count;i++)
	{
		if(strcmp(sp->data[i],elt)==0)
		{
			return i;
		}
	}
	return -1;
}
