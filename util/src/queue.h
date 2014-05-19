/* ========================================================================== */
/* File: queue.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * This file contains the definitions for a priority queue of doc ids.
 *
 */
/* ========================================================================== */
#ifndef QUEUE_H
#define QUEUE_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types

typedef struct QueueNode {
    void *data;				      // pointer to data
    struct QueueNode *prev;                   // pointer to previous node
    struct QueueNode *next;                   // pointer to next node
} QueueNode;

typedef struct Queue {
    QueueNode *head;                          // "beginning" of the list
    QueueNode *tail;                          // "end" of the list
} Queue;

// ---------------- Public Variables

// ---------------- Prototypes/Macros
Queue *createQueue();
void PriorityAdd(Queue *queue, int docid, int freq);
QueueNode *pop(Queue *queue);
void removeNode(QueueNode *node);

#endif // QUEUE.H
