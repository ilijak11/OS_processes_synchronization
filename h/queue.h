/*
 * queue.h
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "PCB.h"

class Queue{
private:
	struct QNode{
		PCB* pcb;
		QNode* next;
		QNode(PCB* _pcb){
			pcb = _pcb;
			next = 0;
		}
	};
	QNode* head;
	QNode* last;
	int size;
	void q_delete();
public:
	Queue();
	~Queue();
	PCB* get();
	void put(PCB* pcb);
	void qprint();
	int find_and_remove(PCB* pcb);
	int is_empty();
};


#endif /* QUEUE_H_ */
