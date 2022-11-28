/*
 * krnlsmph.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */
#include "krnlsmph.h"
#include "SCHEDULE.H"
#include "lock.h"
#include "queue.h"
#include <stdio.h>


KernelSem::KernelSem(int v, Semaphore* ms): val(v), my_sem(ms){
	this->blocked = new Queue();
}

KernelSem::~KernelSem(){
	delete this->blocked;
}

void KernelSem::block(Time time){
	this->blocked->put(PCB::running);
	PCB::running->blocked = 1;
	PCB::running->sem = this;
	if(time != 0){
		PCB::running->blocked_time = time;
		PCB::running->blocked_time_elapsed = 0;
	}
	else{
		PCB::running->blocked_time = -1;
		PCB::running->blocked_time_elapsed = -1;
	}
	//printf("Thread: %d blocked\n", PCB::running->id);
	//asm cli;
	dispatch();
}

void KernelSem::unblock(){
	PCB* pcb = this->blocked->get();
	if(pcb == 0) return;
	pcb->blocked = 0;
	pcb->sem = 0;
	pcb->blocked_time = 0;
	pcb->blocked_time_elapsed = 0;
	//printf("Thread: %d unblocked\n", pcb->id);
	//asm cli;
	Scheduler::put(pcb);
}

int KernelSem::wait(Time time){
	lock;
	if(--this->val < 0) this->block(time);
	unlock;
	if(PCB::running->blocked_time_elapsed == 1) return -1;
	return 1;
}

void KernelSem::signal(){
	lock;
	if(++this->val <= 0) this->unblock();
	unlock;
}

void KernelSem::find_and_unblock(PCB* pcb){
	if(this->blocked->find_and_remove(pcb) == 1){
		pcb->blocked = 0;
		pcb->sem = 0;
		pcb->blocked_time = 0;
		pcb->blocked_time_elapsed = 1;
		this->val++;
		Scheduler::put(pcb);
		//printf("Thread: %d unblocked - time elapsed\n", pcb->id);
	}
	else{
		/*/nesto nije dobro ali radi
		printf("Thread: %d NOT FOUND!\n", pcb->id);
		pcb->blocked = 0;
		pcb->sem = 0;
		pcb->blocked_time = 0;
		pcb->blocked_time_elapsed = 1;
		Scheduler::put(pcb);*/
	}
}

void KernelSem::print(){
	this->blocked->qprint();
}
