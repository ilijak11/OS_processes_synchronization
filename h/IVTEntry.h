/*
 * IVTEntry.h
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"


typedef void interrupt (*pInterrupt)(...);


class KernelEv;

class IVTEntry{
private:
	IVTNo entry;
	pInterrupt old_handler;
	pInterrupt new_handler;
	int exec_old_h;
public:
	static IVTEntry* ivt_entry_array[256];
	static IVTEntry* get_entry_pointer(IVTNo n);
	KernelEv* kernel_event;
	IVTEntry(IVTNo n, pInterrupt n_handler, int exec_old);
	~IVTEntry();

	int exec_old_flag_set() const;
	void exec_old_handler();
	void signal();


};


#endif /* IVTENTRY_H_ */
