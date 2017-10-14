/*****************************************************************************************
Prologue

UCSC Extension: Advanced C Programming

Instructor: Rajainder A. Yeldandi

Author: Kevin Geiszler

Assignment Number: 18, Long Integer

Topics: Advanced Linked Lists

File Name: assignment18_KevinGeiszler.c

Date: 3/9/2017

Objective: This program reads two long integers from an input file and manually performs
		   an addition operation between the two numbers.
		   
Comments:  Each long integer read from the input file is placed into a linked list where
		   its digits are split up into groups of 5 and placed in each link. It is
		   possible for the front link of the list to have less than 5 digits. From right
		   to left, the first link in List 1 is added to the first link in List 2. This
		   sum is placed in the corresponding link in List 3. A carry number may need to
		   be moved over to the next link if the sum of the two links exceeds 99,999.
		   

*****************************************************************************************/

//Preprocessor directives

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256
#define NUMDIGITS 5

struct doubleNode {
	int info;
	struct doubleNode *next;
	struct doubleNode *prev;
};

typedef struct doubleNode *DOUBLENODEPTR;

/****************************************************************************************/

//Function prototypes

int digitCount(char []);
int sectionCount(int);
DOUBLENODEPTR getNode(DOUBLENODEPTR, int);
void freeNode(DOUBLENODEPTR);
DOUBLENODEPTR doubleCreateDigitList(DOUBLENODEPTR, char [], int, int);
DOUBLENODEPTR doubleInsertAtHead(DOUBLENODEPTR, int);
void printDoubleList(FILE *, DOUBLENODEPTR, int);
DOUBLENODEPTR setTail(DOUBLENODEPTR);
int determineListSizes(DOUBLENODEPTR *, DOUBLENODEPTR *, int, int);
void addLongIntegers(FILE *, DOUBLENODEPTR, DOUBLENODEPTR, DOUBLENODEPTR *, int);
void freeDoubleList(DOUBLENODEPTR *);

/****************************************************************************************/

//The main function

int main(int argc, char *argv[])
{
	char myStr1[SIZE]; //48 digits
	char myStr2[SIZE]; //38 digits
	int count1 = 0;
	int count2 = 0;
	int sections1, sections2, listSize;
	
	DOUBLENODEPTR doubleHead1 = NULL;
	DOUBLENODEPTR doubleHead2 = NULL;
	DOUBLENODEPTR doubleHead3 = NULL;
	
	DOUBLENODEPTR doubleTail1 = NULL;
	DOUBLENODEPTR doubleTail2 = NULL;
	
	DOUBLENODEPTR temp = NULL;
	
	FILE *pInFile, *pOutFile;
	
	//check command line arguments
	if (argc != 3)
	{
		printf("Error, need the following file usage: %s inputFile outputFile\n", argv[0]);
		exit(1);
	}
	
	//check if the input file was successfully opened
	if ((pInFile = fopen(argv[1], "r")) == NULL)
	{
		printf("Error opening input file (%s)\n", argv[1]);
		exit(1);
	}
	
	//check if the output file was successfully opened
	if ((pOutFile = fopen(argv[2], "w")) == NULL)
	{
		printf("Error opening output file (%s)\n", argv[2]);
		exit(1);
	}
	
	rewind(pInFile);
	
	//scan the input file and place each long integer into a string
	while(fscanf(pInFile, "%s%s", &myStr1, &myStr2) != 2)
		printf("Error reading input file (%s)\n", argv[1]);
		
	fclose(pInFile);
	
	printf("The following long integer will be read into List 1: %s\n\n", myStr1);
	fprintf(pOutFile, "The following long integer will be read into List 1: %s\n\n", myStr1);
	
	printf("The following long integer will be read into List 2: %s\n\n", myStr2);
	fprintf(pOutFile, "The following long integer will be read into List 2: %s\n\n", myStr2);
	
	//count the number of digits in each long integer string
	count1 = digitCount(myStr1); 
	count2 = digitCount(myStr2);
	
	//count how many 5 digit sections each linked list will have
	//it's possible for the left most node of a linked list to have less than 5 digits
	sections1 = sectionCount(count1);
	sections2 = sectionCount(count2);

	//place 5 digits in each link of a linked list to represent the long integer
	doubleHead1 = doubleCreateDigitList(doubleHead1, myStr1, sections1, count1);
	doubleHead2 = doubleCreateDigitList(doubleHead2, myStr2, sections2, count2);

	//determine how many links each linked list has
	//if one list has fewer links, then additional links filled with zeros are added
	listSize = determineListSizes(&doubleHead1, &doubleHead2, sections1, sections2);
	
	printDoubleList(pOutFile, doubleHead1, 1);
	printDoubleList(pOutFile, doubleHead2, 2);
	
	//traverse to the last link of each list
	doubleTail1 = setTail(doubleHead1);
	doubleTail2 = setTail(doubleHead2);
	
	//preform long integer addition
	addLongIntegers(pOutFile, doubleTail1, doubleTail2, &doubleHead3, listSize);
	
	printf("Here is the result list - ");
	fprintf(pOutFile, "Here is the result list - ");
	printDoubleList(pOutFile, doubleHead3, 3);
	
	//free List 1
	freeDoubleList(&doubleHead1);
	
	doubleHead1 = NULL;
	
	//free List 2
	freeDoubleList(&doubleHead2);
	
	doubleHead2 = NULL;
	
	fclose(pOutFile);

	return 0;
}

/****************************************************************************************/

//Count the number of digits in a long integer.

int digitCount(char str[])
{
	int i = 0;
	int count = 0;

	while (str[i] != '\0')
	{
		count++;
		i++;
	}
	
	return count;
}

/****************************************************************************************/

//Count the number of 5 digit sections a long integer can be split up into.

int sectionCount(int count)
{	
	if (count % NUMDIGITS == 0)
		return(count / NUMDIGITS);
	else
		return(count / NUMDIGITS + 1);
}

/****************************************************************************************/

//Create a new node and return a pointer to it.

DOUBLENODEPTR getNode(DOUBLENODEPTR doubleHead, int x)
{
	DOUBLENODEPTR newNode = (DOUBLENODEPTR)malloc(sizeof(struct doubleNode));
	newNode->info = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	
	return newNode;
}

/****************************************************************************************/

//Free a node.

void freeNode(DOUBLENODEPTR p)
{
	free(p);
}

/****************************************************************************************/

//Create a doubly linked list that represents a long integer. Each link contains 5 digits.

DOUBLENODEPTR doubleCreateDigitList(DOUBLENODEPTR doubleHead, char str[], int sections, int count)
{
	int i, num;
	char group[NUMDIGITS+1];
	
	for (i = 0; i < sections; i++)
	{
		if (count-(NUMDIGITS*(i+1)) >= 0)
		{
			strncpy(group, str+count-(NUMDIGITS*(i+1)), NUMDIGITS);
			group[NUMDIGITS] = '\0';
			num = atoi(group);
			doubleHead = doubleInsertAtHead(doubleHead, num);
		}
		else
		{
			strncpy(group, str, count % NUMDIGITS);
			group[count % NUMDIGITS] = '\0';
			num = atoi(group);
			doubleHead = doubleInsertAtHead(doubleHead, num);
		}
	}
	
	return doubleHead;
}

/****************************************************************************************/

//Insert a node at the beginning of the doubly linked list

DOUBLENODEPTR doubleInsertAtHead(DOUBLENODEPTR doubleHead, int x)
{
	DOUBLENODEPTR newNode = getNode(doubleHead, x);
	
	if(doubleHead == NULL)
	{
		doubleHead = newNode;
		
		return doubleHead;
	}
	
	doubleHead->prev = newNode;
	newNode->next = doubleHead; 
	doubleHead = newNode;
	
	return doubleHead;
}

/****************************************************************************************/

//Print the linked list.

void printDoubleList(FILE *pOut, DOUBLENODEPTR doubleHead, int listNum)
{
	DOUBLENODEPTR temp = doubleHead;
	
	printf("List %d:\n\n", listNum);
	fprintf(pOut, "List %d:\n\n", listNum);
	
	printf("%5d ",temp->info);
	fprintf(pOut, "%5d ",temp->info);
	
	temp = temp->next;
	
	while(temp != NULL)
	{
		printf("%05d ",temp->info);
		fprintf(pOut, "%05d ",temp->info);
		
		temp = temp->next;
	}
	
	printf("\n\n");
	fprintf(pOut, "\n\n");
}

/****************************************************************************************/

//Set a pointer to point to the tail of the linked list.

DOUBLENODEPTR setTail(DOUBLENODEPTR currPtr)
{
	while (currPtr->next != NULL)
		currPtr = currPtr->next;
		
	return currPtr;
}

/****************************************************************************************/

//Perform addition of two simulated long integers. Print each step along the way.

void addLongIntegers(FILE *pOut, DOUBLENODEPTR doubleTail1, DOUBLENODEPTR doubleTail2, DOUBLENODEPTR *doubleHead3, int listSize)
{
	int sum    = 0;
	int number = 0;
	int carry  = 0;
	int count = 1;
	
	printf("Starting from the end of the linked lists and traversing right to left:\n\n");
	fprintf(pOut, "Starting from the end of the linked lists and traversing right to left:\n\n");
	
	printf("-----------------------------------------------------------------------\n\n");
	fprintf(pOut, "-----------------------------------------------------------------------\n\n");

	while ((doubleTail1 != NULL) && (doubleTail2 != NULL))
	{
		printf("Reading (List 1, Link %d): %d\n", count, doubleTail1->info);
		fprintf(pOut, "Reading (List 1, Link %d): %d\n", count, doubleTail1->info);
		
		printf("Reading (List 2, Link %d): %d\n\n", count, doubleTail2->info);
		fprintf(pOut, "Reading (List 2, Link %d): %d\n\n", count, doubleTail2->info);
		
		sum    = doubleTail1->info + doubleTail2->info + carry;
		
		if (count == 1)
		{
			printf("This is the first link, so carry = 0\n\n");
			fprintf(pOut, "This is the first link, so carry = 0\n\n");
		}
		else
		{
			printf("From the previous link, carry = %d\n\n", carry);
			fprintf(pOut, "From the previous link, carry = %d\n\n", carry);
		}
		
		printf("sum = (List 1, Link %d) + (List2, Link %d) + carry = %d + %d + %d = %d\n\n",
						  count, count, doubleTail1->info, doubleTail2->info, carry, sum);
		fprintf(pOut, "sum = (List 1, Link %d) + (List2, Link %d) + carry = %d + %d + %d = %d\n\n",
						  count, count, doubleTail1->info, doubleTail2->info, carry, sum);
		
		number = sum % 100000;
		carry  = sum / 100000;
		
		printf("number = sum %% 1000000 =  %d %% 100000 = %d\n", sum, number);
		fprintf(pOut, "number = sum %% 1000000 =  %d %% 100000 = %d\n", sum, number);
						  
		printf("Placing %d in (List 3, Link %d)\n\n", number, count);
		fprintf(pOut, "Placing %d in (List 3, Link %d)\n\n", number, count);
		
		printf("carry = sum / 100000 = %d / 100000 = %d\n", sum, carry);
		fprintf(pOut, "carry = sum / 100000 = %d / 100000 = %d\n", sum, carry);
		
		printf("Carrying %d over to the next link\n\n", carry);
		fprintf(pOut, "Carrying %d over to the next link\n\n", carry);
		
		printf("-----------------------------------------------------------------------\n\n");
		fprintf(pOut, "-----------------------------------------------------------------------\n\n");
		
		*doubleHead3 = doubleInsertAtHead(*doubleHead3, number);
		doubleTail1 = doubleTail1->prev;
		doubleTail2 = doubleTail2->prev;
		
		//if carry > 0 when the addition is complete, then we must add an extra link to
		//the front of our result list so carry can be inserted there
		if ((count == listSize) && (carry > 0))
		{
			printf("Since carry > 0 at the last link, we must add an extra link to "
											 "List 3 that will contain carry's value.\n\n");
			fprintf(pOut, "Since carry > 0 at the last link, we must add an extra link to "
											 "List 3 that will contain carry's value.\n\n");								 
			
			printf("Adding carry = %d to the beginning of (List 3, Link %d)\n\n", carry, count+1);
			fprintf(pOut, "Adding carry = %d to the beginning of (List 3, Link %d)\n\n", carry, count+1);
			
			printf("-----------------------------------------------------------------------\n\n");
			fprintf(pOut, "-----------------------------------------------------------------------\n\n");
			
			*doubleHead3 = doubleInsertAtHead(*doubleHead3, carry);
		}
		
		count++;
	}
}

/****************************************************************************************/

//Determine if one list is longer than another. If the lists do not have the same number
//of links, then the smaller list will have additional links filled with zeros placed in
//the front. It's possible for the front node of a list to have less than 5 digits.

int determineListSizes(DOUBLENODEPTR *doubleHead1, DOUBLENODEPTR *doubleHead2, int sections1, int sections2)
{
	if (sections1 > sections2)
	{
		for (int i = 0; i < sections1 - sections2; i++)
			*doubleHead2 = doubleInsertAtHead(*doubleHead2, 0);
		
		return sections1;
	}
	else if (sections1 < sections2)
	{
		for (int i = 0; i < sections2 - sections1; i++)
			*doubleHead1 = doubleInsertAtHead(*doubleHead1, 0);
			
		return sections2;
	}
	else
		return sections1;
}

/****************************************************************************************/

void freeDoubleList(DOUBLENODEPTR *doubleHead)
{
	DOUBLENODEPTR temp = NULL;
	DOUBLENODEPTR currPtr = *doubleHead;
	
	while (currPtr != NULL)
	{
		temp = currPtr;
		currPtr = currPtr->next;
		freeNode(temp);
	}
}

/****************************************************************************************/














































