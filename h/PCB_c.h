/*
 * PCB_c.h
 *
 *  Created on: Aug 9, 2021
 *      Author: OS1
 */

#ifndef PCB_C_H_
#define PCB_C_H_
#include "PCB.h"

class PCBCollection{
private:

	struct Node{
		PCB* pcb;
		Node* next;

		Node(PCB* p){
			pcb = p;
			next = 0;
		}
	};

	Node* head;
	Node* last;
public:
	PCBCollection(): head(0), last(0){}
	~PCBCollection();

	static PCBCollection* pcb_collection;

	static void add(PCB* new_pcb);
		void addPCB(PCB* new_pcb);

		static void remove(PCB* pcb);
		void removePCB(PCB* pcb);

		void printPCB();
		static void print();

		void update_blck_time();
		static void update_blocked_time();

		void deleteCollection();

		Thread* findThr(ID id);
		static Thread* find(ID id);
};



#endif /* PCB_C_H_ */
