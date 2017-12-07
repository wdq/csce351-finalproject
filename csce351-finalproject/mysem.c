/*
 * mysem.c
 *
 *  Created on: Nov 12, 2017
 *      Author: witty
 */

#include "mysem.h"
#include <stdio.h>

// Returns 1 if initialization is successful
// Returns 0 if it fails.
// Pass in a semaphore and its initial value.
// The function initializes the semaphore's internal data fields.
int semInit(semaphore * sem, int semVal)
{
	sem->threadCount = 0;
	sem->value = semVal;
	sem->queue = (BQ_type) {NULL, NULL, 0};
	/* add your code to initialize your semaphore here */
	return 1;
}

// Check to see if the semaphore value is greater than 0.
// If so, it decrements the value and just continues.
// If the value is 0, the thread is put to sleep without completing the down for the moment.
// As such, the semaphore value never falls below 0.
void semDown(semaphore * sem)
{
	if(semValue(sem) == 0) {
		mythread_block_self(&sem); // block yourself
		while(1) { // burn off the rest of your quantum
			//if(semValue(sem) > 0) {
			//	break;
			//}
		}
	}
	unsigned int currentSemValue = semValue(sem);

	if(currentSemValue > 0) {
		sem->value = currentSemValue - 1;
	}
	/* implement your logic to perform down operation on a semaphore here */
}


// Increment the value of the semaphore.
// If there are threads sleeping on the semaphore, they all should be unblocked, and they will all compete to complete the down operation.
// As such, after an up on a semaphore with threads sleeping on it, the semaphore value is still 0, but there are no more sleeping threads.
void semUp(semaphore * sem)
{
	printf("A\n");
	sem->value = semValue(sem) + 1;
	printf("B\n");
	mythread_unblock_sem(&sem);
	printf("I\n");
	/* implement your logic to perform up operation on a semaphore here */
}

// Return the current value of the semaphore.
// I think this function does everything it needs to do already.
unsigned int semValue(semaphore * sem)
{
	return sem->value;
}

// Return the number of threads sleeping on the semaphore.
// I think this function does everything it needs to do already.
unsigned int semThreadCount(semaphore * sem)
{
	return sem->threadCount;
}


