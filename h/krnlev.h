/*
 * krnlev.h
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#ifndef KRNLEV_H_
#define KRNLEV_H_

#include "PCB.h"
#include "event.h"
#include "IVTEntry.h"

class KernelEv{
private:
	int val;
	PCB* blocked;
	IVTNo entry_no;
	IVTEntry* entry;
	PCB* creator;


	void block();
	void unblock();
public:
	KernelEv(IVTNo n);
	void signal();
	void wait();
};



#endif /* KRNLEV_H_ */
