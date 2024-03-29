/* 
    Name:   thread_handler.h
    Author: Dongyuan Zhan
    Date:   11/20/2010

Description:
    Functions used to manage threads
*/

#ifndef _THREAD_HANDLER_H_
#define _THREAD_HANDLER_H_

/* possible thread states */
enum tstate {NEW, READY, RUNNING, BLOCKED, TERMINATED, NUM_TSTATES};

/* thread control block */
typedef struct
{
    unsigned int tid;
    unsigned int *stack;
    unsigned int stack_size;
    unsigned int *stack_pointer;
    enum tstate  state;
} tcb;

tcb *mythread_get_current_running_thread();

/* declaration */
tcb *mythread_create(unsigned int tid, unsigned int stack_size, void (*mythread)(unsigned int tid));

void mythread_start(tcb *thread_pointer);

void mythread_join(tcb *thread_pointer);

void mythread_block(tcb *thread_pointer);

void mythread_terminate(tcb *thread_pointer);

void *mythread_schedule(void *context);

unsigned int mythread_isQempty();

void mythread_cleanup();

#endif
