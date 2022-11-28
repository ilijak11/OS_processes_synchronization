/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */
#include <dos.h>
#include <stdio.h>
#include "IVTEntry.h"
#include "lock.h"
#include "krnlev.h"

//inicijalizacija statickog niza nulama
IVTEntry* IVTEntry::ivt_entry_array[256] = {0};

IVTEntry::IVTEntry(IVTNo n, pInterrupt n_handler, int exec_old): entry(n), new_handler(n_handler),
		exec_old_h(exec_old), kernel_event(0){
	lock;
	if(IVTEntry::ivt_entry_array[n] != 0){
		printf("Entry already exists!\n");
		asm cli;
		unlock;
		return;
	}
	else{
		IVTEntry::ivt_entry_array[n] = this;
	}
	this->old_handler = getvect(n);
	setvect(n, n_handler);
	printf("New entry created\n");
	unlock;
}

IVTEntry::~IVTEntry(){
	lock;
	IVTEntry::ivt_entry_array[this->entry] = 0;
	setvect(this->entry, this->old_handler);
	unlock;
}

IVTEntry* IVTEntry::get_entry_pointer(IVTNo n){
	return IVTEntry::ivt_entry_array[n];
}

int IVTEntry::exec_old_flag_set() const{
	return exec_old_h;
}

void IVTEntry::exec_old_handler(){
	this->old_handler();
}


void IVTEntry::signal(){
	if(this->kernel_event != 0) this->kernel_event->signal();
}


