/*
 * Evan Chou Project 6 huffman.c
 */

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include"pqueue.h"
#include"pack.h"

//node structure
typedef struct node NODE;

//declaration
int counts[257]; //array that shows the occurences of each letter/character
NODE *nodes[257]; //nodes array becomes priority queue

/*
 * Function: *mknode
 * Summary: makes parent node and then links right and left child to it
 * Runtime: O(1)
 */
NODE *mknode(int data, NODE *left, NODE *right)
{
	NODE *np=malloc(sizeof(NODE)); //allocating memory for NODE

	//checks if argument is valid
	assert(np!=NULL);

	np->count=data; //number of occurences equal to node's data
	np->parent=NULL; //parent is equal to NULL because no parent has yet to be set

	//left and right nodes should be the new node's children if they are not NULL
	if(left!=NULL) 
	{
		left->parent=np;
	}
	if(right!=NULL)
	{
		right->parent=np;
	}
	return np;
}

/*
 * Function: nodecmp
 * Summary: compares and returns the bigger of the two nodes
 * Runtime: O(1)
 */
int nodecmp(NODE *t1, NODE *t2)
{
	assert(t1!=NULL && t2!=NULL); //checks if argument is valid
	return(t1->count<t2->count)?-1:(t1->count>t2->count); //compares and returns the bigger node
}

/*
 * Function: depth
 * Summary: returns depth of a given node
 * Runtime: O(h)
 */
int depth(NODE *np)
{
	//checks if argument is valid
	assert(np!=NULL);

	//initialize and declare variable
	int counts1;
	counts1=0;

	while(np->parent!=NULL)
	{
		np=np->parent;
		counts1++;
	}
	return counts1; //returns depth of node
}

/*
 * Function: main
 * Summary: main function for huffman tree implementation
 * Runtime: O(n)
 */
int main(int argc, char* argv[])
{
	FILE *fp=fopen(argv[1], "r"); //create file and read

	//return function and tell user if file cannot open
	if(fp==NULL)
	{
		printf("Error in opening file. File cannot be opened.\n");
		return 0;
	}

	//declare and initialize variable
	int c;
	c=getc(fp); //retrieves character at certain spot in the file

	//incrementing character count in array
	while(c!=EOF)
	{
		counts[c]++;
		c=getc(fp);
	}
	fclose(fp); //closing file

	//create priority queue
   	PQ *pq = createQueue(nodecmp);

	//initialize
	int i;

	//add character to priority queue
	for(i=0;i<256;i++)
	{
		if(counts[i] > 0) //ensures there is a character in array
		{
			nodes[i] = mknode(counts[i], NULL, NULL);
			addEntry(pq, nodes[i]); 
	   	}
	}

	//ensures priority queue is never empty, last spot is always equal to 0 for EOF
	nodes[256]=mknode(0, NULL, NULL);

	//this adds the zero leaf
	addEntry(pq, nodes[256]);

	//implement huffman tree
	while(numEntries(pq) > 1)
	{
		//combine least frequent nodes until we are left with one node in the queue
	   	NODE *l=removeEntry(pq);
		NODE *r=removeEntry(pq);
		NODE *p=mknode(l->count+r->count, l, r);
		addEntry(pq, p);
	}

	//initialize
	int j;

	//print out bits for each character in textfile
	for(j=0;j<257;j++)
	{ 
	   	if(nodes[j]!=NULL)
		{
		  	printf(isprint(j) ? "%c" : "%03o", j);
			printf(": %d x %d bits = %d bits\n", counts[j], depth(nodes[j]), counts[j]*depth(nodes[j]));
		}
  	}

	pack(argv[1], argv[2], nodes);
	destroyQueue(pq);
}
