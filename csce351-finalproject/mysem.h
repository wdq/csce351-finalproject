/*
 * mysem.h
 *
 *  Created on: Nov 12, 2017
 *      Author: witty
 */

#ifndef MYSEM_H_
#define MYSEM_H_

#include "blocked_queue.h"

struct mySem{
	int threadCount;
	int value;
	BQ_type queue;
};

typedef struct mySem semaphore;

int semInit(semaphore *, int semValue);
void semDown(semaphore *);
void semUp(semaphore *);
unsigned int semValue(semaphore *);
unsigned int semThreadCount(semaphore *);


#endif /* MYSEM_H_ */
