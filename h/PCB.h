/*
 * PCB.h
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "krnlsmph.h"
#include "queue.h"

class Queue;

class PCB{
public:
	//PCB ID
	static ID s_id;
	ID id;
	//stack
	unsigned* stack;
	//context
	unsigned sp;
	unsigned ss;
	unsigned bp;
	//execution time
	Time exec_time;
	Time time_left;
	int zero_time;
	int elapsed();
	//semaphore time
	Time blocked_time;
	int blocked_time_elapsed;
	//semaphore on which thread is blocked
	KernelSem* sem;
	//status/mode
	int started;
	int finished;
	int blocked;
	int waiting_to_complete;
	int event_blocked;
	int mode;
	//Thread
	friend class Thread;
	Thread* my_thread;
	//wrapper
	static void wrapper();
	//running
	static PCB* running;
//private:
	PCB(StackSize ss = defaultStackSize, Time ts = defaultTimeSlice, Thread* my_t);
	~PCB();
	//threads that are waiting for this thread to finish
	Queue* join_queue;
	//test
	void print();
	void wait_to_complete();
	void resume_waiting_to_comeplete();
};



#endif /* PCB_H_ */
