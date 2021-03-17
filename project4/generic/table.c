/*
 * Evan Chou Project 4 Table for Generic
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
	void **data; //pointing to array of character pointers that each point to a string
	char *flag; //keeps track of which indexes have what status
	int (*compare)(); //pointer to cmopare function
	unsigned (*hash)(); //pointer to hash function
}SET;

/*
 * Function: search
 * Summary: searches for specific element
 * Runtime: Expected/Worst: O(1)/O(n)
 */
static unsigned  search(SET *sp, void *elt, bool *found)
{
	//initialize variable
	unsigned int index;
	unsigned int start;
	int temp=0;
	int save=-1;
	int i=0;
	
	start=(*sp->hash)(elt); //number unique to key
	start=(start%(sp->length)); //home address for given key 
	
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
			if((*sp->compare)(sp->data[index],elt)==0)
			{
				*found=true;
				return index;
			}
		}
		if(sp->flag[index]==DELETED && temp==0)
		{
			save=index;
			temp=1; //prevents the save from being updated after the first delete is encountered
		}
	}
	*found=false; //spot never found
	return save;
}

/*
 * Function: *createSet
 * Summary: function for creating set
 * Runtime: O(n)
 */
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	//initialize variable
	int i;

	//check function argument restrictions
	assert(compare!=NULL);
	assert(hash!=NULL);

	SET *sp; //pointint to structure SET
	sp=malloc(sizeof(SET));
	assert(sp!=NULL);
	assert(maxElts>0);
	sp->count=0; //number of strings
	sp->length=maxElts; //max number elements given as maxElts
	sp->data=malloc(sizeof(void*)*maxElts); //allocate memory for array to strings
	assert(sp->data!=NULL);
	sp->flag=malloc(sizeof(char)*maxElts); //allocate for strings keeping track of index
	assert(sp->flag!=NULL);
	sp->compare=compare; //pointer to compare function with set
	sp->hash=hash; //pointer to hash function

	//flag array to have empty or E slots
	for(i=0;i<maxElts;i++)
	{
		sp->flag[i]=EMPTY;
	}
	return sp;
}

/*
 * Function: destroySet
 * Summary: function for deleting set data
 * Runtime: O(1)
 */
void destroySet(SET *sp)
{
	//check if set is empty
	assert(sp!=NULL);

	//free pointers
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
 * Function: addElements
 * Summary: function will input new string into set
 * Runtime: Expected/Worse: O(1)/O(n)
 */
void addElement(SET *sp,void *elt)
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
	sp->data[index]=elt; //string duplicate to copy string and return pointer
	sp->flag[index]=FILLED; //flag is filled
	sp->count++;
	return;
}

/*
 * Function: removeElement
 * Summary: function will remove given string from set
 * Runtime: Expected/Worst: O(1)/O(n)
 */
void removeElement(SET *sp,void *elt)
{
	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);

	//inialize
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
	return;
}

/*
 * Function: *findElement
 * Summary: function will return specific string
 * Runtime: Expected/Worst: O(1)/O(n)
 */ 
void *findElement(SET *sp,void *elt)
{
	//checks if set is empty
	assert(sp!=NULL);
	assert(elt!=NULL);
	
	//initialize
	bool found=false;

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
 * Function: *getElements
 * Summary: function will return identical set pointer
 * Runtime: O(n)
 */
void *getElements(SET *sp)
{
	//initialize
	int i,j=0;
	void **temp;
	assert(sp!=NULL);

	temp=malloc(sizeof(void*)*sp->count);
	assert(temp!=NULL);

	//copies memory for new array of character pointers
	for (i=0;i<sp->length;i++)
	{
		if(sp->flag[i]==FILLED)
		{
			temp[j]=sp->data[i];
			j++;
		}
	}
	return temp;
}
