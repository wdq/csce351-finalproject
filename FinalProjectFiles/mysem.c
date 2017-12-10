/*
 * mysem.c
 *
 *  Created on: Nov 12, 2017
 *      Author: witty
 */

#include "mysem.h"
#include "thread_handler.h"
#include <stdio.h>

int semInit(semaphore * sem, int semVal)
{
	Q_type *semQueue = (Q_type *) malloc(sizeof(Q_type));
	semQueue->head = NULL;
	semQueue->size = 0;
	semQueue->tail = NULL;
	sem->block_queue = semQueue;
	sem->threadCount = 0;
	sem->value = semVal;
	return 1;
}

void semDown(semaphore * sem)
{
	/* implement your logic to perform down operation on a semaphore here */
	while(1) {
		asm("wrctl status, zero"); // disable interrupts
		//sem->value--;
		int retry = 0;
		tcb *thread = mythread_get_current_running_thread();
		//printf("semDown start from thread #%d, value=%d\n", thread->tid, sem->value);
		if(sem->value == 0) {
			//printf("Blocking (tid=%d)\n", thread->tid);
			thread->state = BLOCKED;
			//unsigned int *tmp_ptr;
			//tmp_ptr = thread->stack_pointer;
			//tmp_ptr[0] = (unsigned int)semDown;
			enqueueFrontQ((void *)thread, sem->block_queue);
			sem->threadCount++;
			retry = 1;
		} else {
			sem->value--;
			asm("movi et, 1"); // enable interrupts
			asm("wrctl status, et");
			break;
		}
		asm("movi et, 1"); // enable interrupts
		asm("wrctl status, et");

		while(thread->state == BLOCKED) {
			// burn off quantum
		}


		//asm("wrctl status, zero"); // disable interrupts
		//unsigned int *tmp_ptr2;
		//tmp_ptr2 = thread->stack_pointer;
		//if(tmp_ptr2[0] != (unsigned int)mythread_cleanup) {
		//	tmp_ptr2[0] = (unsigned int)mythread_cleanup;
		//}

		//asm("movi et, 1"); // enable interrupts
		//asm("wrctl status, et");

		//printf("I'm not blocked anymore (tid=%d)\n", thread->tid);
		//if(retry == 1) {
			//printf("semDown retry from thread #%d\n", thread->tid);
		//	semDown(sem);
		//}

		//printf("semDown finish from thread #%d\n", thread->tid);
	}
}

void semUp(semaphore * sem)
{
	/* implement your logic to perform up operation on a semaphore here */
	asm("wrctl status, zero"); // disable interrupts
	tcb *thread = mythread_get_current_running_thread();
	//printf("semUp start from thread #%d, value=%d\n", thread->tid, sem->value);
	sem->value++;
	if(sem->threadCount > 0) {
		while(sem->threadCount > 0) {
			tcb *blockedThread = (tcb *)dequeueQ(sem->block_queue);
			if(blockedThread != NULL) {
				//printf("Unblocking (tid=%d)\n", blockedThread->tid);
				blockedThread->state = READY;
				enqueueFront(blockedThread);
				sem->threadCount--;
			}
		}
	}
    asm("movi et, 1"); // enable interrupts
    asm("wrctl status, et");
    //printf("semUp finish from thread #%d\n", thread->tid);
}

unsigned int semValue(semaphore * sem)
{
	return sem->value;
}

unsigned int semThreadCount(semaphore * sem)
{
	return sem->threadCount;
}


