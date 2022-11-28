/*
 * PCB.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */
#include "PCB.h"
#include "lock.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>


ID PCB::s_id = 1;
PCB* PCB::running = 0;


void PCB::wrapper(){
	if(PCB::running != 0 && PCB::running->my_thread != 0) PCB::running->my_thread->run();
	lock;
	PCB::running->finished = 1;
	printf("THR %d - finished\n", PCB::running->id);
	asm cli;
	PCB::running->resume_waiting_to_comeplete();
	dispatch();
	unlock;
}

PCB::PCB(StackSize sts, Time ts, Thread* thr): my_thread(thr), mode(0), finished(0),
		blocked(0), blocked_time(0), blocked_time_elapsed(0),
		waiting_to_complete(0), started(0), event_blocked(0){
	this->join_queue = new Queue();
	this->id = s_id++;
	this->sem = 0;
	if(ts != 0){
		this->exec_time = ts;
		this->time_left = ts;
		this->zero_time = 0;
	}
	else{
		this->exec_time = -1;
		this->time_left = -1;
		this->zero_time = 1;
	}
	if(sts != 0){
		sts /= sizeof(unsigned);
		this->stack = new unsigned[sts];
		this->stack[sts - 1] = 0x200;
		this->stack[sts - 2] = FP_SEG(PCB::wrapper);
		this->stack[sts - 3] = FP_OFF(PCB::wrapper);
		this->ss = FP_SEG(this->stack + sts - 12);
		this->sp = FP_OFF(this->stack + sts - 12);
		this->bp = this->sp;
		this->mode = 1;
	}
	else{
		this->ss = 0;
		this->sp = 0;
		this->bp = 0;
	}
}

int PCB::elapsed(){
	if(this->zero_time == 1) return 0;
	if(this->time_left <= 0){
		this->time_left = this->exec_time;
		return 1;
	}
	this->time_left--;
	return 0;
}

PCB::~PCB(){
	delete this->join_queue;
	delete [] this->stack;
}

void PCB::print(){
	printf("PCB ID: %d ||", this->id);
}

void PCB::wait_to_complete(){
	lock;
	if(this->finished == 1){
		unlock;
		return;
	}
	this->join_queue->put(PCB::running);
	printf("Thr: %d is waiting fot Thr: %d to finish\n", PCB::running->id, this->id);
	asm cli;
	PCB::running->waiting_to_complete = 1;
	dispatch();
	unlock;
}

void PCB::resume_waiting_to_comeplete(){
	while(this->join_queue->is_empty() == 0){
		lock;
		PCB* tmp = this->join_queue->get();
		printf("Thr: %d - resuming\n", tmp->id);
		asm cli;
		tmp->waiting_to_complete = 0;
		if(tmp->finished == 0 && tmp->blocked == 0){
			Scheduler::put(tmp);
		}
		unlock;
	}
}

