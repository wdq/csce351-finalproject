/* 
    Name:   queue.h
    Author: Dongyuan Zhan
    Date:   11/20/2010

Description:
    Functions used to implment a queue and its operations
*/

// I modified the queue to have additional functions that accept the queue as an input.
// That way a queue can be stored with the semaphore and still modified in here.
// I modified the existing functions so that they use the extended functions to reduce duplicate code.

#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct element_type
{
    void                *data;
    struct element_type *next;
} E_type;

typedef struct queue_type
{
    E_type              *head;
    E_type              *tail;
    unsigned int        size;
} Q_type;

void enqueue(void *data);

void enqueueQ(void *data, Q_type *inqueue);

void *dequeue();

void *dequeueQ(Q_type *inqueue);

unsigned int getQsize();

unsigned int getQsizeQ(Q_type *inqueue);

#endif
