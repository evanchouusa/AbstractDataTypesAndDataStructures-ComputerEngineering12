/*
 * Evan Chou Term Project 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
#include"list.h"
#define LENGTH 8  
#define NODE struct node

//create node structure
struct node
{
	NODE *next;
	NODE *prev;
	void **data; //this is for our array in circular qeueu, data stored
	int count; //number of slots filled in array
	int length; //length of the array in node or total length
	int first; //slot number or location of first item (index of first item)
};

//create list structure
typedef struct list
{
	int count; //numItems, total count in list
	NODE *head; 
	NODE *tail;
}LIST;

/*
 * Function: *createList
 * Summary: function for creating list, returns a pointer to a new list
 * Runtime: O(1)
 */
LIST *createList(void)
{
	LIST *lp; //pointing to the structure LIST
	lp=malloc(sizeof(LIST));
	assert(lp!=NULL);
	lp->count=0; //initial count for list is 0

	NODE *temp=malloc(sizeof(NODE));
	assert(temp != NULL);
	temp->next=NULL;
	temp->prev=NULL;
	temp->data=(void **)malloc(sizeof(void*)*LENGTH);
	temp->count=0; //initial count for item in node is 0
	temp->length=LENGTH; //node length=8
	temp->first=0; //first position for item is set initially at index 0

	//set both tail and head pointer to temp node
	lp->tail=lp->head=temp;

	return lp;
}

/*
 * Function: destroyList
 * Summary: function for deleting list data, will deallocate memory associated with the list pointed to by lp
 * Runtime: O(n)
 */
void destroyList(LIST *lp)
{
	//checks if list is empty
	assert(lp!=NULL);

	//initialize and assign pointers 
	NODE *pDel, *pNext;
	pDel=lp->head;
	pNext=lp->head;

	while(pDel!=NULL)
	{
		pNext=pDel->next;
		free(pDel->data);
		free(pDel);
		pDel=pNext;
	}
	free(lp); //dellocating memory
}

/* Function: numItems
 * Summary: function will return the number of items in the list pointed to by lp
 * Runtime: O(1)
 */
int numItems(LIST *lp)
{
	assert(lp!=NULL);
	return lp->count;
}

/* Function: addFirst
 * Summary: function will add new item as the first item in the list pointed to by lp
 * Runtime: O(1)
 */
void addFirst(LIST *lp, void *item)
{
	//checking if list and item are valid and if they are empty
	assert(lp!=NULL && item!=NULL);

	NODE *current;
	current=lp->head;

	//add new node 
	if(current->count==current->length) //full, so add new NODE in front
	{
		NODE *temp=malloc(sizeof(NODE));
		assert(temp != NULL);
		temp->next=current;
		temp->prev=NULL;
		temp->data=(void **)malloc(sizeof(void*)*current->length*2); //we are doubling each new node length to ease the complexity of running large files
		temp->count=0; //no items in new node
		temp->length=current->length*2;
		temp->first=0; //set first to index 0

		//move list head pointer to new created node, so that node is now the first in the list
		lp->head=temp;
		current->prev=temp;
		current=temp;
	}
	
	//if it is not full, add item to front of array in node
	current->first=(current->first+current->length-1) % current->length; //this will ensure item always gets added in front of original first item index
	current->data[current->first]=item;

	//increment count since we added an item
	current->count++; 
	lp->count++; 
}

/*
 * Function: addLast
 * Summary: function will add new item as the last item in the list pointed to by lp
 * Runtime: O(1)
 */
void addLast(LIST *lp, void *item)
{
	//checking if list and item are valid and if they are empty
	assert(lp!=NULL && item!=NULL);

	NODE *current;
	current=lp->tail;

	//add new node
	if(current->count==current->length) //full, so add new NODE to last or end of the list
	{
		NODE *temp=malloc(sizeof(NODE));
		assert(temp != NULL);
		temp->next=NULL;
		temp->prev=current;
		temp->data=(void **)malloc(sizeof(void*)*current->length*2); //we are doubling each new node length to ease the complexity of running large files
		temp->count=0; //no items in new node
		temp->length=current->length*2;
		temp->first=0; //set first to index 0

		//move list tail pointer to new created node, so that node is now the last in the list
		lp->tail=temp;
		current->next=temp;
		current=temp;
	}

	//if it is not full, add item to end of array in node
	current->data[(current->first+current->count) % current->length]=item; //this will ensure item always gets added after the original last item index

	//increment count since we added an item
	current->count++;
	lp->count++;
}

/*
 * Function: *removeFirst
 * Summary: function will remove and return the first item in the list pointed to by lp, the list must not be empty
 * Runtime: O(1)
 */
void *removeFirst(LIST *lp)
{
	//checks if list is empty
	assert(lp!=NULL && lp->count>0);

	//initialize and create pointer
	NODE *pDel; //pointer to eventually removed value
	pDel=lp->head; //assign pDel as the first node in list 

	if(pDel->count==0) //this is if there is nothing inside the node
	{
		lp->head=pDel->next;
		lp->head->prev=NULL;
		free(pDel->data); 
		free(pDel);
		pDel=lp->head;
	}

	void *first=pDel->data[pDel->first];  //pointer for returning value
	pDel->first=(pDel->first+1) % pDel->length; //this ensures that the first item in the list gets removed

	//decrement count since we deleted an item
	pDel->count--; 
	lp->count--;
	return first;
}

/*
 * Function: *removeLast
 * Summary: function will remove and return the last item in the list pointed to by lp, the list must not be empty
 * Runtime: O(1)
 */
void *removeLast(LIST *lp)
{
	//checks if list is empty
	assert(lp!=NULL && lp->count>0); 

	//initialize and create pointer
	NODE *pDel; //pointer to eventually removed value
	pDel=lp->tail; //assign pDel as the last node in the list

	if(pDel->count==0) //this is if there is nothing inside the node
	{
		lp->tail=pDel->prev;
		lp->tail->next=NULL;
		free(pDel->data);
		free(pDel);
		pDel=lp->tail;
	}

	void *last=pDel->data[(pDel->first+pDel->count-1) % pDel->length]; //pointer for returning value, this also ensures that the last item in the list gets removed

	//decrement count since we deleted an item
	pDel->count--;
	lp->count--;
	return last;
}

/*
 * Function: *getItem
 * Summary: function will return the item at position index in the list pointed to by lp, the index must be within range
 * Runtime: O(n)
 */
void *getItem(LIST *lp, int index)
{
	//checks if list is empty and within range
	assert(lp!=NULL);

	//initialize and create pointer
	NODE *current;
	current=lp->head; //assign current node as the head node in list

	while(index>=current->count) //everytime we traverse, the index decreases
	{
		index-=current->count;
		current=current->next;
	}	
	return current->data[(current->first+index)%current->length]; //this ensures that the item at the wanted index will be returned
}

/*
 * Function: setItem
 * Summary: function will update the item at position index in the list pointed to by lp, the index must be within range
 * Runtime: O(n)
 */
void setItem(LIST *lp, int index, void *item)
{
	//checks if list is empty and within range
	assert(lp!=NULL);

	//initialize and create pointer
	NODE *current;
	current=lp->head;

	while(index>=current->count) //everytime we traverse, the index decreases
	{
		index-=current->count;
		current=current->next;
	}
	current->data[(current->first+index)%current->length]=item; //this ensures that the item will be updated at the specific index 
}

