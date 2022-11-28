/*
 * thread.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */
#include "thread.h"
#include "PCB_c.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "lock.h"
#include "timer.h"
#include <stdlib.h>
#include <stdio.h>

Thread::Thread(StackSize ss, Time ts){
	this->myPCB = new PCB(ss, ts, this);
	PCBCollection::add(this->myPCB);
}

Thread::~Thread(){
	waitToComplete();
	PCBCollection::remove(this->myPCB);
	delete this->myPCB;
}

void Thread::start(){
	if(this->myPCB->started == 0){
		this->myPCB->started = 1;
		Scheduler::put(this->myPCB);
	}
}


ID Thread::getId(){
	return this->myPCB->id;
}

ID Thread::getRunningId(){
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id){
	return PCBCollection::find(id);
}

void Thread::waitToComplete(){
	myPCB->wait_to_complete();
}

volatile int on_demand = 0;

void dispatch(){
	lock;
	on_demand = 1;
	timerHandler();
	unlock;
}
