/*
 * Evan Chou Project 6 pqueue.c
 */

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"pqueue.h"
#include<stdbool.h>
#define LENGTH 10 //initial size of array before reallocations

//define macros to help find parents and children
#define p(x) (((x)-1)/2)
#define l(x) ((x)*2+1)
#define r(x) ((x)*2+2)


//pq structure
struct pqueue
{
    int count; //number of entries in array
    int length; //length of allocated array
    void **data; //allocated array of entries
    int (*compare)(); //comparison function
};

/*
 * Function: *createQueue
 * Summary: returns a pointer to a new priority queue using compare as its comparison function
 * Runtime: O(1)
 */
PQ *createQueue(int (*compare)())
{
    PQ *pq; //pointing to structure pqueue

	assert(compare!=NULL);

    pq=malloc(sizeof(PQ)); //allocating memory for structure
    assert(pq!=NULL);

	pq->compare=compare;
	pq->length=LENGTH; 
    pq->count=0; //initial count for pq is 0

    pq->data=malloc(sizeof(void *) * pq->length);
    assert(pq->data!=NULL);

    return pq;
}

/*
 * Function: destroyQueue
 * Summary: deallocates memory associated with the priority queue pointed to by pq
 * Runtime: O(n)
 */
void destroyQueue(PQ *pq)
{
	//checks if empty and argument is valid
    assert(pq!=NULL);

	//initialize
	int i;

	for(i=0;i<pq->count;i++)
	{
		free(pq->data[i]);
	}

	//free array pointer and then structure pointer
    free(pq->data);
    free(pq);
    return;
}

/*
 * Function: numEntries
 * Summary: returns the number of entries in the priority queue pointed to by pq
 * Runtime: O(1)
 */
int numEntries(PQ *pq)
{
    assert(pq!=NULL);
    return pq->count;
}

/*
 * Function: addEntry
 * Summary: add entry to the priority queue pointed to by pq
 * Runtime: O(logn)
 */
void addEntry(PQ *pq, void *entry)
{
	//checks if queue is valid 
    assert(pq!=NULL);
   
    //if the array is full, then double array size 
    if(pq->count+1>pq->length)
    {
        pq->data=realloc(pq->data,sizeof(void *)*pq->length*2); //doubling array length to create space
        pq->length=pq->length*2; //now the new array length is doubled the original
    }

    //insert new element at end of binary heap
    pq->data[pq->count]=entry;

	//index variable initialization and declaration
    int index; 
	index=pq->count; 
    pq->count++;

	//parent variable initialization and declaration
    int parent;
	parent=p(index);

    //add new element at index while it is within the boundary
    while(parent>=0)
    {
        if((*pq->compare)(pq->data[parent],pq->data[index])>0)
        {
            void *temp=pq->data[parent];
            pq->data[parent]=pq->data[index];
            pq->data[index]=temp;
            index=parent;
        }
        else
        {
            break;
        }
        parent=p(index); //new parent location
    }
    return;
}

/*
 * Function: removeEntry
 * Summary: remove and return the smallest entry from the priority queue pointed to by pq
 * Runtime: O(logn)
 */
void *removeEntry(PQ *pq)
{
	//checks if queue is valid
    assert(pq!=NULL);

    void *root; //pointer for root of tree
	void *temp; //temporary pointer for storage and swap

	//initialize and declare variables
	int check; //variable for checking and comparing each element
	int parent; //variable for parents of tree
	int new; //variable for new parent

    root=pq->data[0]; //set root equal to first element in array
    parent=0; //initially no parents, so parent count is 0
    new=0; //initially non new parents, so new parent count is 0

	//remove smallest entry
    pq->data[0]=pq->data[pq->count-1];
    pq->count--; //decrement count after removal

	//re-assigning after removal
    while(l(parent)<pq->count)
    {
        if(r(parent)<pq->count)
        {
            check=(*pq->compare)(pq->data[l(parent)],pq->data[r(parent)]);
            if(check<0)
            {
                new=l(parent);
            }
            else
            {
                new=r(parent);
            }
        }
        else
        {
            new=l(parent);
        }
        if(((*pq->compare)(pq->data[parent],pq->data[new]))>0)
        {
			//swapping 
            temp=pq->data[parent];
            pq->data[parent]=pq->data[new];
            pq->data[new]=temp;
        }
        else
        {
            break;
        }
        parent=new;
    }
    return root; //return smalles entry
}
