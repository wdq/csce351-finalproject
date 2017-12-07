#ifndef _BLOCKED_QUEUE_H_
#define _BLOCKED_QUEUE_H_

typedef struct belement_type
{
    void                *data;
    struct belement_type *next;
} BE_type;

typedef struct bqueue_type
{
    BE_type              *head;
    BE_type              *tail;
    unsigned int        size;
} BQ_type;

void benqueue(void *data, BQ_type *queue);

void *bdequeue(BQ_type *queue);

unsigned int bgetQsize(BQ_type *queue);

#endif
