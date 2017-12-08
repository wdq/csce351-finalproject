#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "alarm_handler.h"
#include "thread_handler.h"
#include "queue.h"
#include "blocked_queue.h"
#include "mysem.h"

#define DELAY 10000
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* The two macros are extremely useful by turnning on/off interrupts when atomicity is required */
#define DISABLE_INTERRUPTS() {  \
    asm("wrctl status, zero");  \
}

#define ENABLE_INTERRUPTS() {   \
    asm("movi et, 1");          \
    asm("wrctl status, et");    \
}

/* the current running thread */
static tcb *current_running_thread      = NULL;


/* pointing to the stack/context of main() */
static unsigned int *main_stack_pointer = NULL;

tcb *mythread_create(unsigned int tid, unsigned int stack_size, void (*mythread)(unsigned int tid))
{
    unsigned int *tmp_ptr;
    
    /* allocate a tcb for a thread */
    tcb *thread_pointer;
    
    thread_pointer                      = (tcb *)malloc(sizeof(tcb));
    if (thread_pointer == NULL)
    {
        printf("Unable to allocate space!\n");
        exit(1);
    }
    
    /* initialize the thread's tcb */
    thread_pointer->tid                 = tid;
    thread_pointer->stack               = (unsigned int *)malloc(sizeof(unsigned int) * stack_size);
    if (thread_pointer->stack == NULL)
    {
        printf("Unable to allocate space!\n");
        exit(1);
    }
    thread_pointer->stack_size          = stack_size;
    thread_pointer->stack_pointer       = (unsigned int *)(thread_pointer->stack + stack_size - 19);
    thread_pointer->state               = NEW;
    
    /* initialize the thread's stack */
    tmp_ptr                             = thread_pointer->stack_pointer;
    tmp_ptr[18]                         = (unsigned int)mythread;                               // ea
    tmp_ptr[17]                         = 1;                                                    // estatus
    tmp_ptr[5]                          = tid;                                                  // r4
    tmp_ptr[0]                          = (unsigned int)mythread_cleanup;                       // ra
    tmp_ptr[-1]                         = (unsigned int)(thread_pointer->stack + stack_size);   // fp
           
    return thread_pointer;
}

/* NEW ----> READY */
void mythread_start(tcb *thread_pointer)
{
    // assert(thread_pointer && thread_pointer->state == NEW);
    thread_pointer->state = READY;
}

/* READY --push into--> readyQ */
void mythread_join(tcb *thread_pointer)
{
    // assert(thread_pointer && thread_pointer->state == READY);
    enqueue((void *)thread_pointer);
}

/* RUNNING ----> BLOCKED */
void mythread_block(tcb *thread_pointer)
{
    // assert(thread_pointer && thread_pointer->state == RUNNING);
    thread_pointer->state = BLOCKED;
}

// Block the thread the calls this function.
void mythread_block_self(semaphore * sem) {
	asm("wrctl status, zero");
	//DISABLE_INTERRUPTS();
	//printf("A-0: %d\n", current_running_thread->tid);
	if(current_running_thread != NULL) {
		//printf("A-0-0\n");
		current_running_thread->state = BLOCKED; // set yourself to blocked
	    benqueue(current_running_thread, &sem->queue); // add yourself to the blocked queue
	    //printf("A-0-1\n");
	    //dequeue(current_running_thread); //
	}
    asm("movi et, 1");
    asm("wrctl status, et");
	//ENABLE_INTERRUPTS();
}

// Unblock all of the threads that are waiting on a semaphore.
void mythread_unblock_sem(semaphore * sem) {
	asm("wrctl status, zero");
	//DISABLE_INTERRUPTS();
	printf("C: tid=%d, bqsize=%d\n", current_running_thread->tid, bgetQsize(&sem->queue));
	tcb *dequeuedThread = NULL;
	while(bgetQsize(&sem->queue) > 0) { // Go until there are no more blocked threads in the queue
		dequeuedThread = bdequeue(&sem->queue);
		//printf("D\n");
		if(dequeuedThread == NULL) {
			//printf("D-1\n");
			break;
		}
		//tcb *dequeuedThread = bdequeue(&sem->queue); // dequeue one thread
		//printf("E\n");
		dequeuedThread->state = READY; // set it to the ready state
		//printf("F\n");
		enqueue(dequeuedThread); // add it back to the run queue
		//printf("G\n");
	}
	//printf("H\n");
	//ENABLE_INTERRUPTS();
    asm("movi et, 1");
    asm("wrctl status, et");
}

tcb *mythread_get_current_thread() {
	return current_running_thread;
}

/* RUNNING ----> TERMINATED */
void mythread_terminate(tcb *thread_pointer)
{
    // assert(thread_pointer && thread_pointer->state == RUNNING);
    thread_pointer->state = TERMINATED;
}

void *mythread_schedule(void *context)
{
	printf("mythread_schedule() running (queue size=%d) (status=%d) (tid=%d)\n", getQsize(), current_running_thread->state, current_running_thread->tid);
    if (getQsize() > 0)
    {
        if (current_running_thread != NULL) // if there is already a thread running, set it to ready, and add to the queue
        {
        	printf("S-A\n");
        	if(current_running_thread->state == BLOCKED) {
        		current_running_thread->stack_pointer = (unsigned int *)context;
        		printf("S-A-1\n");
        	} else {
        		printf("S-A-2\n");
				// assert(current_running_thread->state == RUNNING);
				// assert(main_stack_pointer != NULL);
				current_running_thread->state = READY;
				current_running_thread->stack_pointer = (unsigned int *)context;
				enqueue(current_running_thread);
        	}
        }
        else if (main_stack_pointer == NULL) // if there isn't already a thread running, don't do anything with the current thread
        {
        	printf("S-B\n");
            main_stack_pointer = (unsigned int *)context;
        }
        
        current_running_thread = (tcb *)dequeue(); // get a new thread to run from the queue
        // assert(current_running_thread->state == READY);
        //if(current_running_thread->state == BLOCKED) {
        //	mythread_schedule(&context);

        //}
        current_running_thread->state = RUNNING;
        
        context = (void *)(current_running_thread->stack_pointer);
    }
    else if (current_running_thread==NULL && main_stack_pointer!=NULL)
    {        
        context = (void *)main_stack_pointer;
    }

    return context;
}

unsigned int mythread_isQempty()
{
    return (getQsize() == 0) && (current_running_thread == NULL);
}

void mythread_cleanup()
{
	int count = 0;
    DISABLE_INTERRUPTS();
    mythread_terminate(current_running_thread);
    free(current_running_thread->stack);
    free(current_running_thread);
    current_running_thread = NULL;
    ENABLE_INTERRUPTS();
    while(1){
    	for (count = 0; count < DELAY; count++);
    	printf(".");
    }


}
