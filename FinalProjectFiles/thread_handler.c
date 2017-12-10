#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "alarm_handler.h"
#include "thread_handler.h"
#include "queue.h"

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

tcb *mythread_get_current_running_thread() {
	return current_running_thread;
}

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

/* RUNNING ----> TERMINATED */
void mythread_terminate(tcb *thread_pointer)
{
    // assert(thread_pointer && thread_pointer->state == RUNNING);
    thread_pointer->state = TERMINATED;
}

void *mythread_schedule(void *context)
{
	//printf("mythread_schedule() running (queue size=%d) (status=%d) (tid=%d)\n", getQsize(), current_running_thread->state, current_running_thread->tid);
	//printf("mythread_schedule(), (current tid=%d), (queue size=%d)\n", current_running_thread->tid, getQsize());
    if (getQsize() > 0)
    {
    	//printf("mythread_schedule() A\n");
        if (current_running_thread != NULL && current_running_thread->state != BLOCKED)//&& current_running_thread->state != BLOCKED)
        {
        	//printf("mythread_schedule() B\n");
            // assert(current_running_thread->state == RUNNING);
            // assert(main_stack_pointer != NULL);
            current_running_thread->state = READY;
            current_running_thread->stack_pointer = (unsigned int *)context;
            enqueue(current_running_thread);
        }
        else if(current_running_thread != NULL && current_running_thread->state == BLOCKED) {
        	current_running_thread->stack_pointer = (unsigned int *)context;
        }
        else if (main_stack_pointer == NULL)
        {
        	//printf("mythread_schedule() C\n");
            main_stack_pointer = (unsigned int *)context;
        }
        
        current_running_thread = (tcb *)dequeue();
        // assert(current_running_thread->state == READY);
        current_running_thread->state = RUNNING;
        
        context = (void *)(current_running_thread->stack_pointer);
        //printf("mythread_schedule() D\n");
    }
    else if (current_running_thread==NULL && main_stack_pointer!=NULL)
    {        
    	//printf("mythread_schedule() E\n");
        context = (void *)main_stack_pointer;
    }
    //printf("mythread_schedule() F\n");
    //printf("state=%d, \n", current_running_thread->state);
    //printf("--------------------------------running tid=%d--------------------------------\n", current_running_thread->tid);

    return context;
}

unsigned int mythread_isQempty()
{
    return (getQsize() == 0) && (current_running_thread == NULL);
}

void mythread_cleanup()
{
	//printf("mythread_cleanup(), (tid=%d)\n", current_running_thread->tid);
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