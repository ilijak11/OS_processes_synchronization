/*
 * Event.h
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned char IVTNo;

class KernelEv;
#include "IVTEntry.h"

#define PREPAREENTRY(num, flag)\
	void interrupt inter##num(...);\
	IVTEntry entry##num(num, inter##num, flag);\
	void interrupt inter##num(...){\
		entry##num.signal();\
		if(entry##num.exec_old_flag_set()) entry##num.exec_old_handler();\
	}


class Event {
public:
 Event (IVTNo ivtNo);
 ~Event ();
 void wait ();
protected:
 friend class KernelEv;
 void signal(); // can call KernelEv
private:
 KernelEv* myImpl;
};



#endif /* EVENT_H_ */
