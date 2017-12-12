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
		int retry = 0;
		tcb *thread = mythread_get_current_running_thread();
		if(sem->value == 0) {
			thread->state = BLOCKED;
			enqueueQ((void *)thread, sem->block_queue);
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
	}
}

void semUp(semaphore * sem)
{
	/* implement your logic to perform up operation on a semaphore here */
	asm("wrctl status, zero"); // disable interrupts
	tcb *thread = mythread_get_current_running_thread();
	sem->value++;
	if(sem->threadCount > 0) {
		while(sem->threadCount > 0) {
			tcb *blockedThread = (tcb *)dequeueQ(sem->block_queue);
			if(blockedThread != NULL) {
				blockedThread->state = READY;
				enqueue(blockedThread);
				sem->threadCount--;
			}
		}
	}
    asm("movi et, 1"); // enable interrupts
    asm("wrctl status, et");
}

unsigned int semValue(semaphore * sem)
{
	return sem->value;
}

unsigned int semThreadCount(semaphore * sem)
{
	return sem->threadCount;
}


