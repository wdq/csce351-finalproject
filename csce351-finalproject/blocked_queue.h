typedef struct element_type
{
    void                *data;
    struct element_type *next;
} BE_type;

typedef struct queue_type
{
    BE_type              *head;
    BE_type              *tail;
    unsigned int        size;
} BQ_type;

void enqueue(void *data);

void *dequeue();

unsigned int getQsize();
