#include <stdio.h>
#include "queue.h"

static Q_type queue = {NULL, NULL, 0};

void enqueue(void *data)
{
	enqueueQ(data, &queue);
}

void enqueueQ(void *data, Q_type *inqueue) {
    E_type  *elem;

    if ((elem = (E_type *)malloc(sizeof(E_type))) == NULL)
    {
        printf("Unable to allocate space!\n");
        exit(1);
    }
    elem->data = data;
    elem->next = NULL;

    if (inqueue->head == NULL)
    	inqueue->head = elem;
    else
    	inqueue->tail->next = elem;
    inqueue->tail = elem;

    inqueue->size++;
}

void *dequeue()
{
	dequeueQ(&queue);
}

void *dequeueQ(Q_type *inqueue) {
    E_type  *elem;
    void    *data = NULL;

    if (inqueue->size != 0)
    {
        elem = inqueue->head;
        if (inqueue->size == 1)
        	inqueue->tail = NULL;
        inqueue->head = inqueue->head->next;

        inqueue->size--;
        data = elem->data;
        free(elem);
    }

    return data;
}

unsigned int getQsize()
{
    return getQsizeQ(&queue);
}

unsigned int getQsizeQ(Q_type *inqueue) {
	return inqueue->size;
}

