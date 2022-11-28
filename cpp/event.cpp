/*
 * event.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */
#include "event.h"
#include "krnlev.h"

Event::Event(IVTNo n){
	this->myImpl = new KernelEv(n);
}

Event::~Event(){
	delete this->myImpl;
}

void Event::wait(){
	this->myImpl->wait();
}

void Event::signal(){
	this->myImpl->signal();
}


