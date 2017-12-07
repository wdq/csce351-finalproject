#include <stdio.h>
#include "blocked_queue.h"

//static BQ_type queue = {NULL, NULL, 0};

#define DISABLE_INTERRUPTS() {  \
    asm("wrctl status, zero");  \
}

#define ENABLE_INTERRUPTS() {   \
    asm("movi et, 1");          \
    asm("wrctl status, et");    \
}

void benqueue(void *data, BQ_type *queue)
{
    BE_type  *elem;

    if ((elem = (BE_type *)malloc(sizeof(BE_type))) == NULL)
    {
        printf("Unable to allocate space!\n");
        exit(1);
    }
    elem->data = data;
    elem->next = NULL;

    if (queue->head == NULL)
        queue->head = elem;
    else
        queue->tail->next = elem;
    queue->tail = elem;

    queue->size = queue->size + 1;
}

void *bdequeue(BQ_type *queue)
{
    BE_type  *elem;
    void    *data = NULL;

    if (queue->size != 0)
    {
        elem = queue->head;
        if (queue->size == 1)
            queue->tail = NULL;
        queue->head = queue->head->next;

        queue->size = queue->size - 1;
        data = elem->data;
        free(elem);
    }

    return data;
}

unsigned int bgetQsize(BQ_type *queue)
{
    return queue->size;
}
