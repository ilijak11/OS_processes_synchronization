/*
 * krnlev.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */
#include <stdio.h>
#include "krnlev.h"
#include "lock.h"
#include "SCHEDULE.H"


KernelEv::KernelEv(IVTNo n): entry_no(n), val(0){
	this->entry = IVTEntry::get_entry_pointer(n);
	if(this->entry != 0) this->entry->kernel_event = this;
	this->creator = PCB::running;
}


void KernelEv::block(){
	this->blocked = PCB::running;
	PCB::running->event_blocked = 1;
	//printf("Thread: %d waiting for event\n", PCB::running->id);
	//asm cli;
	dispatch();
}

void KernelEv::unblock(){
	this->blocked->event_blocked = 0;
	Scheduler::put(this->blocked);
	this->blocked = 0;
}

void KernelEv::wait(){
	lock;
	if(PCB::running == this->creator){
		if(this->val == 0) this->block();
		else this->val = 0;
	}
	unlock;
}

void KernelEv::signal(){
	lock;
	if(this->blocked == 0) this->val = 1;
	else{
		this->unblock();
		this->val = 0;
	}
	unlock;
}
