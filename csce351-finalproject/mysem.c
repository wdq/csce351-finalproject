/*
 * mysem.c
 *
 *  Created on: Nov 12, 2017
 *      Author: witty
 */

#include "mysem.h"

// Returns 1 if initialization is successful
// Returns 0 if it fails.
// Pass in a semaphore and its initial value.
// The function initializes the semaphore's internal data fields.
int semInit(semaphore * sem, int semVal)
{
	//sem.queue =
	sem.threadCount = 0;
	sem.value = semVal;
	/* add your code to initialize your semaphore here */
	return 1;
}

// Check to see if the semaphore value is greater than 0.
// If so, it decrements the value and just continues.
// If the value is 0, the thread is put to sleep without completing the down for the moment.
// As such, the semaphore value never falls below 0.
void semDown(semaphore * sem)
{
	/* implement your logic to perform down operation on a semaphore here */
}


// Increment the value of the semaphore.
// If there are threads sleeping on the semaphore, they all should be unblocked, and they will all compete to complete the down operation.
// As such, after an up on a semaphore with threads sleeping on it, the semaphore value is still 0, but there are no more sleeping threads.
void semUp(semaphore * sem)
{
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


