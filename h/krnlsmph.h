/*
 * krnlsmph.h
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */

#ifndef KRNLSMPH_H_
#define KRNLSMPH_H_

#include "semaphor.h"
//#include "queue.h"
#include "PCB.h"

class Queue;

class KernelSem{
private:
	int val;
	Semaphore* my_sem;
	Queue* blocked;

	void block(Time time);
	void unblock();

public:
	KernelSem(int val, Semaphore* my_sem);
	~KernelSem();
	int get_val(){
		return this->val;
	}

	int wait(Time t2w);
	void signal();
	void find_and_unblock(PCB* pcb);
	void print();

};




#endif /* KRNLSMPH_H_ */
