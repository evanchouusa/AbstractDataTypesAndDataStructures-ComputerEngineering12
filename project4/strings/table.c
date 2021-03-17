/*
 * Evan Chou Project 4 Table for Strings
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"set.h"
#include<stdbool.h>
#define EMPTY 0
#define FILLED 1
#define DELETED 2

typedef struct set
{
	int count; //number of strings held in the string array or set
	int length; //length of array of character pointers and max number of elements allowed
	char **data; //pointing to array of character pointers that each point to a string
	char *flag; //keeps track of which indexes have what status
}SET;

/*
 * Function: strhash
 * Summary: function will return numerator to yeild index of a given key
 * Runtime: O(1)
 */
unsigned strhash(char *elt)
{
	unsigned hash=0;
	while(*elt!='\0')
	{
		hash = 31 * hash + *elt++;
	}
	return hash;
}

/*
 * Function: search
 * Summary: searches for specific element
 * Runtime: Expected/Worst: O(1)/O(n)
 */
static unsigned search (SET *sp, char *elt, bool *found)
{
	//initialize variable
	unsigned int index;
	unsigned int start;
	int temp=0;
	int save=-1;
	int i=0;

	start=strhash(elt);//number unique to key
	start=(start%(sp->length));//home adress for given key 

	for(int i=0;i<sp->length;i++)
	{
		index=(start+i)%sp->length;
		if(sp->flag[index]==EMPTY)
		{
			if(save!=-1)
			{
				*found=false;
				return save;
			}
			else
			{
				*found=false;
				return index;
			}
		}
		if(sp->flag[index]==FILLED)
		{
			if(strcmp(sp->data[index],elt)==0)
			{
				*found=true;
				return index;
			}
		}
		if(sp->flag[index]==DELETED && temp==0)
		{
			save=index;
			temp=1;//prevents the save from being updated after the first delete is encountered
		}
	}
	*found=false;//spot never found
	return save;
}

/*
 * Function: *createSet
 * Summary: function for creating set
 * Runtime: O(n)
 */
SET *createSet(int maxElts)
{
	//initialize variable
	int i;

	SET *sp; //pointing to structure SET
	sp=malloc(sizeof(SET));
	assert(sp!=NULL);
	assert(maxElts>0);
	sp->count=0; //number of strings
	sp->length=maxElts; //max number elements given as maxElts
	sp->data=malloc(sizeof(char*)*maxElts); //allocate memory for array to strings
	assert(sp->data!=NULL);
	sp->flag=malloc(sizeof(char)*maxElts);//allocate for strings keeping track of index
	assert(sp->flag!=NULL);

	//flag array to have emtpy or E slots
	for(i=0;i<maxElts;i++)
	{
		sp->flag[i]=EMPTY;
	}
	return sp;
}

/*
 * Function: destroySet
 * Summary: function for deleting set data
 * Runtime: O(n)
 */
void destroySet(SET *sp)
{
	//initialize 
	int i;

	//checks if set is empty
	assert(sp!=NULL);

	//free pointer for each string in character pointer array to delete all strings
	for(i=0;i<sp->length;i++)
	{
		if(sp->flag[i]==FILLED)
		{
			free(sp->data[i]);
		}
	}
	free(sp->data);
	free(sp->flag);
	free(sp);
	return;
}

/*
 * Function: numElements
 * Summary: function will return number of elements in the set
 * Runtime: O(1)
 */
int numElements(SET *sp)
{
	assert(sp!=NULL);
	return sp->count;
}

/*
 * Function: addElement
 * Summary: function will input new string into set
 * Runtime: Expected/Worst: O(1)/O(n)
 */
void addElement(SET *sp,char *elt)
{   
	//checks if the element number already equals to the array maximum, then cannot add
	assert(sp->count<sp->length);
	assert(sp!=NULL);
	assert(elt!=NULL);

	//initialize
	bool found=false;
	unsigned index;

	//makes sure inserted element is unique
	index=search(sp,elt,&found);
	if(found==true)
	{
		return;
	}

	//define elt
	elt=strdup(elt);

	//this is only if element is unique
	assert(elt!=NULL);
	sp->data[index]=elt;//string duplicate to copy string and return pointer
	sp->flag[index]=FILLED;//flag is filled
	sp->count++;
	return;
}

/*
 * Function: removeElement
 * Summary: function will remove given string from set
 * Runtime: Expected/Worst: O(1)/O(n)
 */
void removeElement(SET *sp,char *elt)
{
	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);

	//initialize
	bool found = false;

	//search for index of specific string
	unsigned index=search(sp,elt,&found);

	//check to see if specific string exists
	if(found==false)
	{
		return;
	}
	sp->count--;
	sp->flag[index]=DELETED;
	free(sp->data[index]);
	return;
}

/*
 * Function: *findElement
 * Summary: function will return specific string
 * Runtime: Expected/Worst: O(1)/O(n)
 */
char *findElement(SET *sp,char *elt)
{
	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);
	
	//initialize
	bool found;

	//search for index of specific string
	int index=search(sp,elt, &found);

	//checks to see if specific string exists
	if(found==false)
	{
		return NULL;
	}
	return sp->data[index]; 
}

/*
 * Function: **getElements
 * Summary: function will return identical set pointer
 * Runtime: O(n)
 */
char **getElements(SET *sp)
{
	//initialize
	int i,j=0;
	char **temp;
	assert(sp!=NULL);

	temp=malloc(sizeof(char*)*sp->count);
	assert(temp!=NULL);
	
	//copies memory for new array of character pointers
	for (i=0;i<sp->length;i++)
	{
		if(sp->flag[i]==FILLED)
		{
			temp[j]=strdup(sp->data[i]);
			j++;
		}
	}
	return temp;
}
