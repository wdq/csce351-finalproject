#include <stdio.h>
#include "queue.h"

// Created a copy of this (blocked_queue.c) to handle the blocked queue.
// The provided queue seems to just be something to run the thread_handler stuff.
//

static Q_type queue = {NULL, NULL, 0};

#define DISABLE_INTERRUPTS() {  \
    asm("wrctl status, zero");  \
}

#define ENABLE_INTERRUPTS() {   \
    asm("movi et, 1");          \
    asm("wrctl status, et");    \
}

void enqueue(void *data)
{
    E_type  *elem;
    
    if ((elem = (E_type *)malloc(sizeof(E_type))) == NULL)
    {
        printf("Unable to allocate space!\n");
        exit(1);
    }
    elem->data = data;
    elem->next = NULL;
    
    if (queue.head == NULL)
        queue.head = elem;
    else
        queue.tail->next = elem;
    queue.tail = elem;

    queue.size++;
}

void *dequeue()
{
    E_type  *elem;
    void    *data = NULL;
    
    if (queue.size != 0)
    {
        elem = queue.head;
        if (queue.size == 1)
            queue.tail = NULL;
        queue.head = queue.head->next;
        
        queue.size--;
        data = elem->data;
        free(elem);
    }
        
    return data;
}

unsigned int getQsize()
{
    return queue.size;
}
