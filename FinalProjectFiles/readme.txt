CSCE 351 Final Project - Fall 2017

------------------------------------------------------------------------------------------

Team members:

1. Andrezza De lemos
2. Nicolas Esser
3. William Quade

------------------------------------------------------------------------------------------

Source code files:

1. alarm_handler.(c,h)
	We didn't need to modify this file.
	It seems to enable the processor's clock timer interrupts which drive the thread handler.
	Once the program finishes the "Interrupted by the DE2 timer!" message from this file is printed on the screen.
2. exception_handler.S
	This is an assembly file. We didn't need to modify it.
	It helps the thread handler function by moving around the appropriate registers whenever there is a clock interrupt. 
	It also triggers the mythread_schedule function in the scheduler to run.
3. mysem.(c,h)
	This is where the bulk of our work was done. 
	This file is used in the project1 file to allow the sleeping barber problem to work without running into multithreading problems.
	First the semaphore struct was extended to support a blocked threads queue. 
	The semInit function was extended to default to the value passed to the function, and defaulting to 0/NULL for the other values.
	The semDown function was extended to check if the semaphore value is zero, and blocking the thread if it is.
		Threads are blocked by setting the state, and adding it to the semaphore's block queue. 
		If the semaphore isn't zero, then the function decrements the semaphore and exits. 
		Interrupts are disabled in this function to provide atomicity.
		I found having the semDown function in a while loop allows it to handle multiple threads being released at once.
		Which all fight to get the semDown. That way they all check again once they get unblocked.
	The semUp function was extended to increment the semaphore value, and unblock all blocked threads waiting on the semaphore.
		Interrupts are disabled in this function to provide atomicity.
		The function loops through each thread in the queue, and dequeues it.
		Once dequeued the state is set to ready, and the thread is added to the run queue.
		The threads are then ready to be scheduled by the scheduler. 
	The semValue and semThreadCount functions are simple and just return semaphore properties.
4. project1.c
	This file wasn't really modified.
	The provided code implements the sleeping barber problem. 
	It uses semaphores to handle one barber, a waiting room with 5 chairs, and 15 customers who need haircuts. 
	Each customer and barber has its own thread that runs either a barber or customer loop.
	Once a customer gets a haircut they leave, and once the barber has given 15 haircuts he leaves.
	The MAX value can be modified to speed up the output of the program, which is nice for quickly debugging.
5. queue.(c,h)
	This file was modified, but not a whole lot.
	It was extended to support using both the provided queue that was setup in the file, and queues passed to the function.
	This was done by having enqueue()/dequeue(), and enqueueQ()/dequeueQ() functions. 
		The ones with Q at the end allow a queue to be passed to them.
		The ones without Q use the queue in the file, and pass that queue to the ones with the Q at the end.
	The function are pretty simple enqueue adds an item to the queue, dequeue takes an item from the queue, and getQsize returns the size.
	This file is used by the scheduler, and semaphores to keep track of threads throughout the thread_handler, and mysem files. 
6. thread_handler.(c,h)
	This file implements a round robin scheduler.
	It sets up new threads for running, allows the states of the threads to change, schedules threads, manages the thread run queue, and removes completed threads.
	It was modified a little bit to not schedule blocked threads in the mythread_schedule function.
		This mostly involved just checking for the BLOCKED state before setting the leaving thread as READY and adding it back to the queue. 
		This function takes the currently running thread, saves its state, and then dequeues and sets up the next thread.
		There is also code to handle starting with the main thread, and returning back to it.
	A function was also added called mythread_get_current_running_thread which just returns the running thread.
	This code runs from project1 when creating threads, and from exception_handler when threads get scheduled at clock interrupts.
	
------------------------------------------------------------------------------------------

How to run/compile:

* Nothing weird is going on here. It should run/compile just like the code provided.
* Just import the code into an Nios II Eclipse project, and run it on a DE2 board (it should work with other Altera boards too).
* The instructions on the Canvas class page on how to setup the project should get it running if you are having problems.

------------------------------------------------------------------------------------------

Readme part provided with the other files:

Readme - Final Project Template Files

DESCRIPTION:
Simple program that solves the sleeping barber problem. 
The program is not complete and you'll need to complete mysem.c to implement semaphore.


PERIPHERALS USED:
This example exercises the following peripherals:
- STDOUT device (UART or JTAG UART)

SOFTWARE SOURCE FILES:
This example includes the following software source files:
- hello_world.c: Everyone needs a Hello World program, right?

BOARD/HOST REQUIREMENTS:
This example requires only a JTAG connection with a Nios Development board. If
the host communication settings are changed from JTAG UART (default) to use a
conventional UART, a serial cable between board DB-9 connector  and the host is
required.

------------------------------------------------------------------------------------------
