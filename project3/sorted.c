/*
 * Evan Chou Project 3 Sorted 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdbool.h>
#include"set.h"

//structure for set
typedef struct set
{
	int count; //number of strings held in the string array or set
	int length; //length of array of character pointers and max number of elements allowed
	char **data; //pointing to array of character pointers that each point to a string
}SET;

//private function search prototype
static int search(SET *,char *,bool *);

//function for creating set
SET *createSet(int maxElts)
{
	SET *sp; //pointing to structure SET
	sp=malloc(sizeof(SET)); 
	assert(sp!=NULL); 
	sp->count=0; //initialize
	sp->length=maxElts; // max number of elements given as maxElts
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
	//inialize	
	int index,i;
	
	//element is not found	
	bool found=false;

	//checks if the element number already equals to the array maximum, then cannot add
	assert(sp->count<sp->length);

	//search to see if elt is unique, then add element if it is
	assert(elt!=NULL);
	index=search(sp,elt,&found);
	if(found==true)
	{
		return;
	}

	//shift elements down starting from last one
	for(i=(sp->count-1);i>=index;i--)
	{
		sp->data[i+1]=sp->data[i];
	}
	sp->data[index]=strdup(elt);//string duplicate to copy string and return pointer
	sp->count++;
	return;
}

//function will remove given string from set
void removeElement(SET *sp,char *elt)
{
	//initialize
	int index,i;
	bool found;
	
	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);

	//search for index of specific string
	index=search(sp,elt,&found);

	//checks if string exists
	if(found==false)
	{
		return;
	}
	
	//delete element at index and shift rest of elements up to fill empty spot
	for(i=index;i<=(sp->count-1);i++)
	{
		sp->data[i]=sp->data[i+1];
	}
	sp->data[sp->count-1]=NULL;
	sp->count--;	
	return;
}

//function will return specific string
char *findElement(SET *sp,char *elt)
{
	//initialize
	int index;

	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);
	
	bool found=false;

	//search for index of specific string
	index=search(sp,elt,&found);

	//checks if string exists
	if(found==false)
	{
		return NULL;
	}
	return sp->data[index];
}

//function will return identical set pointer
char **getElements(SET *sp)
{
	assert(sp!=NULL); //checks if set is empty
	char **temp=malloc(sizeof(char*)*sp->count);
	assert(temp!=NULL);

	//copies memory for new array of character pointers
	memcpy(temp,sp->data,sizeof(char*)*sp->count);
	return temp;
}

//searches for specific element
static int  search(SET *sp, char *elt, bool *found)
{
	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);

	//initialize and declare
	int hi,lo,mid;
	lo=0;
	hi=((sp->count)-1);

	//binary search
	while(lo<=hi)
	{
		mid=(hi+lo)/2;
		if(strcmp(elt,sp->data[mid])==0)
		{
			*found=true;
			return mid;
		}
		if(strcmp(elt,sp->data[mid])>0)
		{
			lo=mid+1;
		}
		else
		{
			hi=mid-1;
		}

	}
	*found=false;	
	return lo;
}
