//-----------------------------------------
//// NAME: Ochiagha ifeanyi
//// STUDENT NUMBER: 7900465
//// COURSE: COMP 2160, SECTION: A02
//// INSTRUCTOR: Rasit Eskicioglu 
//// ASSIGNMENT: assignment 4
//// 
//// REMARKS: implementing an Object Manager
////
////-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ObjectManager.h"
Ref refCounter = 1;
int freePtr = 0;
unchar* bufferOne;
unchar* bufferTwo;
unchar* bufferThree;
int initialised  = 0;

typedef struct Node node;

struct Node{
	Ref reference;
	int count;
	int startAdd;
	unlong numBytes;
	node *next;

};
void compact();
void insert(node *);
int checkSpace(int);

node *head = NULL;

//------------------------------------------------------
//// myRoutine
////
//// PURPOSE: creates Nodes
//// INPUT PARAMETERS:
//// it takes in the number of bytes of the object to be created
//// OUTPUT PARAMETERS:
//// this allocates space for the node and creates a node
////------------------------------------------------------
node* createNode(int size)
{
	assert(size >= 0);
	assert(freePtr >= 0);
	node* newNode = (node*) malloc(sizeof(node));
	newNode->reference = refCounter;
	newNode->count = 1;
	newNode->next = NULL;
	newNode->numBytes = size;
	newNode->startAdd = freePtr;
	assert(newNode->reference == refCounter);
	refCounter++;
	assert(newNode->numBytes == size);
	assert(newNode->startAdd == freePtr);
	assert(newNode != NULL);
	return newNode;

}

//this function initializes the object manager using the double buffer method and a pointer to buffer currently in use//
void initPool(){
	initialised = 1;
	bufferTwo = (unchar*) malloc(MEMORY_SIZE);
	bufferOne = bufferTwo;
	bufferThree = NULL;
	assert(bufferOne != NULL);
	assert(bufferThree == NULL);
}

//these are the dbc principles to validate the node upon creation
void validate(node* check){
	assert(check->reference > 0);
	assert(check->numBytes >= 0);
	assert(check->startAdd >= 0);
	assert(check->count > 0);
}

//this function takes a node pointer as an input parameter, it inserts into the head when the list is still empty and then continues inserts at the end of the list//
void insert(node *newNode)
{
	assert(newNode != NULL);
	validate(newNode);

	node* current = NULL;

	if (head == NULL)
	{
		head = newNode;
			
	}
	else {
		
		current = head;
		assert(current != NULL);

		while (current->next != NULL) {//this loops until it reaches the last node in the linked list//
			
			current = current->next;

		}

		current->next = newNode;
		
	}
	
	assert(head != NULL);
	validate(newNode);

}

//this funtion checks if there is stil enough available space in the buffer, it takes in an int as an input parameter and returns an int, 1 if there is space and 0 if there is not//
int checkSpace(int size){
	assert(size >= 0);
	int result = 0;
	if(size + freePtr < MEMORY_SIZE){
		result = 1;
	}
	assert(freePtr >= 0);
	return result;

}

//this function inserts the object into the object manager, it takes in a integer as a parameter and returns a Ref which is a user defined unsigned long//
Ref insertObject(int size){
	assert(size >= 0);
	node *newNode = NULL;
	int result = checkSpace(size);
	Ref refId = NULL_REF;
	if(initialised){//this is a variable declared when the object manager is initialised and has to be true for inserts to happen//
	if(result == 1){
		assert((size + freePtr) < MEMORY_SIZE);
		newNode = createNode(size);
		validate(newNode);
		insert(newNode);
		freePtr = freePtr + size;
		refId = newNode->reference;
		validate(newNode);
		assert(newNode->numBytes == size);
						
	}
	else{
		assert((size + freePtr) > MEMORY_SIZE);//this makes sure the memory is full before calling compact again in the next line//
		compact();
		result = checkSpace(size);
		if(result == 1){
			assert((size + freePtr) < MEMORY_SIZE);
			newNode = createNode(size);
               	        insert(newNode);
               	        freePtr = freePtr + size;
                        refId = newNode->reference;
                        validate(newNode);
                        assert(newNode->numBytes == size);

		}
		else{
			printf("still not enough available space after calling compact");

		}

	}
}
	else{
		printf("object manager has not been initialised");
	}	
	return refId;//returns the reference id if the object has been created and NULL if the object failed to get created//
}

//this function takes a REF as an input parameter and returns a void pointer to the address of the object in the buffer
void* retrieveObject(Ref ref){
	assert(ref > 0);
	node *curr = head;
	node *found = NULL;
	void* pointer = NULL;
	while(curr != NULL){
	assert(curr != NULL);
		if(curr->reference == ref){
			found = curr;
			validate(found);
			pointer = &bufferOne[found->startAdd];

		}
		curr = curr->next;
	}
	if(pointer == NULL){
		printf("object cannot be found");
	}
	return pointer;
}

//this function takes the reference id of the object as a parameter and increases the object count by 1
void addReference(Ref ref){
	assert(ref > 0);
	node *curr = head;
	int inside = 0;
	while(curr != NULL && inside == 0){
		if(curr->reference == ref){
			curr->count = curr->count + 1;
			inside = 1;
		}
		else{	
		curr = curr->next;
		}
	}
	if(inside == 0){
		printf("ref cannot be found\n");
	}				
	else{
		assert(curr->count > 1);
	//	validate(curr);
	}
}

//this function takes the reference id of an object as an input parameter and decreases the count by 1
void dropReference(Ref ref){
	assert(ref >0);
	
	node* curr= head;
	node* prev = NULL;
	if (head != NULL)
	{
		if(ref == head->reference){
			if (head->count == 1)
			{
				head = head->next;//if the count of head is 0, it is no longer in use and so will be deleted from the linked list
			}
			
			else
			{
				head->count--;
			}
			
			
		}
		
		else
		{
			
			while (curr->next != NULL  && ref != curr->reference) {
				
				prev = curr;
				assert(prev != NULL);
		
				curr = curr->next;

			}
			
			if (curr->count == 1) {
				curr->count = curr->count - 1;
				prev->next = curr->next;//when the object count goes down to 0 the object will be deleted from the list//
				free(curr);
				}
			else
				{
				curr->count = curr->count - 1;
				}		
		}
	}
}

//this a destructor for the object manager, it destroys all the nodes in the linked list and frees the space for both the linked list and the buffer currently in use
void destroyPool(){
	

	initialised = 0;
	node* curr = head;
        node* prev = NULL;
        while(curr != NULL){
                prev = curr;
                curr = curr->next;
                free(prev);
        }
        refCounter = 1;
        freePtr = 0;
	head = NULL;
	 if(bufferOne == bufferTwo){
                free(bufferTwo);
        }
        else{
                free(bufferThree);
      	}
	assert(head == NULL);//to check if it is empty//
	
	 
}

//this function displays all the necessary information about an object in the linked list like the ocunt, number of bytes and the start address
void dumpPool(){
	node *current = head;
	while(current != NULL){
		validate(current);
		printf("current object count: %d , current object reference: %lu , current object number of bytes: %lu, current object startAddress: %d\n", current->count, current->reference, current->numBytes, current->startAdd);
		current = current->next;	
}


}

//this function is only called when the buffer in current use is already full and the data needs to be copied from one buffer to another//
void compact(){
	assert(bufferOne != NULL);
	assert(freePtr >= 0);
	int value = freePtr;
	freePtr = 0;
	node* current = head;
	int numObject = 0;
	unlong usedMemory = 0;
	unlong collectedBytes = 0;
	int count = 0;


	if(bufferOne == bufferTwo){//this is checking which of the buffers is currenly in use//
		bufferThree = (unchar*)  malloc(MEMORY_SIZE);//this initialised the buffer we are switching to now the previous buffer is full//
		while(current != NULL && MEMORY_SIZE > freePtr){
			validate(current);
		int result = checkSpace(current->numBytes);
		if(result == 0){
			printf("insufficient space for current object");

		}
		else{
			numObject++;
			for(int i = 0; i < current->numBytes;i++){
				count = current->startAdd + i;
				bufferThree[freePtr++] = bufferOne[count];		
			}
		}
			current = current->next;
		}
		
		bufferOne = bufferThree;
		free(bufferTwo);
	}	 
		
	else{
		
			bufferTwo = (unchar*) malloc(MEMORY_SIZE);
			while (current != NULL)
			{
				validate(current);
				int result = checkSpace(current->numBytes);
				if (result == 0)
				{
					printf("\nNot enough space for current object");

				}
				else
				{
					numObject++;
					for (int i = 0; i < current->numBytes; i++) {
						count  = current->startAdd + i;
						bufferTwo[freePtr++] = bufferOne[count];
					}
				}
				
				current = current->next;

			}
			bufferOne = bufferTwo;
			free(bufferThree);

	}

	assert(freePtr >= 0);
	assert(value >= 0);
	usedMemory = freePtr;
	collectedBytes = value - freePtr;
	assert(usedMemory >= 0);
	assert(numObject >= 0);
	assert(collectedBytes >= 0);
	assert(bufferOne != NULL);
	printf("\nObjects: %d\nbytes in use: %lu\nbytes collected: %lu \n", numObject, usedMemory, collectedBytes);		
}
