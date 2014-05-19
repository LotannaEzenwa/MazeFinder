#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "queue.h"
#include "common.h"
#include "utils.h"

/*
 *  Function to create a new priority queue
 */
Queue * createQueue() {
	Queue *queue=calloc(1,sizeof(Queue));
	queue->head = queue->tail = NULL;
	return queue;	
}

/*
 * Function to add a node to the proper place in the queue
 */
void PriorityAdd(Queue *queue, void *data) {
	// if there's nothing in the queue, create the first node
	if (NULL == queue->tail) {
		QueueNode *firstNode = calloc(1,sizeof(QueueNode));
		firstNode->data = data;
		firstNode->next=NULL;
		firstNode->prev=firstNode;

		// initialize the head and tail to the only item in the queue
		queue->head = firstNode;
		queue->tail = firstNode;
		
	}
	else {

		// create a new node
		// allocate memory
		QueueNode *new = calloc(1,sizeof(QueueNode));
		QueueNode *pointer = queue->head;
		new->data =data;

		// iterate through the queue looking to see where the new node should go
		// queue is in order from greatest to least freq of word
		while (new->freq < pointer->freq) {
			if (pointer->next != NULL) {
				pointer=pointer->next;
			}
			else {
				break;
			}
		}		

		// fix up links
		// if the node is supposed to be inserted at the head (highest priority):
		if (pointer->prev == pointer) {
			queue->head = new;
			new->prev = new;
			new->next = pointer;
			pointer->prev = new;	
		}
		
		// if the node is supposed to be inserted in the middle of the queue:
		else if (pointer != NULL) {
			new->prev = pointer->prev;
			new->next = pointer;
			pointer->prev->next = new;
			pointer->prev = new;
		}

		// if the node is supposed to be inserted at the end (lowest priority):
		else {
			new->prev = pointer;
			new->next = NULL;
			pointer->next = new;
		}
	}
}

/* 
 * Function to remove specific nodes from the queue (without returning anything)
 */
void removeNode(QueueNode *node) {
	
	// if there's something in the node, just splice it out of the queue 
	if (node != NULL) {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
}

/* 
 * Function to remove item from the front of the queue and return it
 */
QueueNode * pop(Queue *queue) {
	
	// check to see if the queue is empty
	if (NULL == queue->head) {
		return NULL;
	}
	else {
		// get the data from the first item in the queue
		QueueNode *first = queue->head;	

		// fix up links
		queue->head=queue->head->next;

		// return the item
		return first;

	}
}	

