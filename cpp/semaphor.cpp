/*
 * semaphor.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */
#include "semaphor.h"
#include "krnlsmph.h"


Semaphore::Semaphore(int init){
	this->myImpl = new KernelSem(init, this);
}

Semaphore::~Semaphore(){
	delete this->myImpl;
}

int Semaphore::wait(Time t2w){
	return this->myImpl->wait(t2w);
}

void Semaphore::signal(){
	this->myImpl->signal();
}

int Semaphore::val() const {
	return this->myImpl->get_val();
}
