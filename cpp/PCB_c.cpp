/*
 * PCB_c.cpp
 *
 *  Created on: Aug 9, 2021
 *      Author: OS1
 */
#include "PCB_c.h"
#include <stdio.h>

PCBCollection* PCBCollection::pcb_collection = new PCBCollection();

void PCBCollection::addPCB(PCB* new_pcb){
	Node* tmp = new Node(new_pcb);
	last = (!head)?head:last->next = tmp;
}

void PCBCollection::add(PCB* new_pcb){
	pcb_collection->addPCB(new_pcb);
}

void PCBCollection::removePCB(PCB* p){
	Node* curr = head;
	Node* prev = 0;
	for(curr = head; curr != 0; prev = curr, curr = curr->next){
		if(curr->pcb == p) break;
	}
	if(curr == 0) return;
	if(curr == head){
		head = head->next;
		delete curr;
	}
	else{
		prev->next = curr->next;
		delete curr;
	}
	return;
}

void PCBCollection::remove(PCB* pcb){
	pcb_collection->removePCB(pcb);
}

Thread* PCBCollection::findThr(ID id){
	for(Node* curr = head; curr != 0; curr = curr->next){
		if(curr->pcb->id == id) return curr->pcb->my_thread;
	}
	return 0;
}

Thread* PCBCollection::find(ID id){
	return pcb_collection->findThr(id);
}

void PCBCollection::deleteCollection(){
	Node* old = 0;
	while(head){
		old = head;
		head = head->next;
		delete old;
	}
}

PCBCollection::~PCBCollection(){
	pcb_collection->deleteCollection();
	delete pcb_collection;
}

void PCBCollection::printPCB(){
	for(Node* curr = head; curr != 0; curr = curr->next){
		printf("PCB: %d |", curr->pcb->id);
	}
	putchar('\n');
}

void PCBCollection::print(){
	pcb_collection->printPCB();
}

void PCBCollection::update_blck_time(){
	for(Node* curr = head; curr != 0; curr = curr->next){
		if(curr->pcb->blocked == 1 && curr->pcb->blocked_time_elapsed != -1){
			//curr->pcb->sem->print();
			curr->pcb->blocked_time--;
			//printf("PCB: %d blocked time left %d\n", curr->pcb->id, curr->pcb->blocked_time);
			asm cli;
			if(curr->pcb->blocked_time == 0){
				curr->pcb->sem->find_and_unblock(curr->pcb);
			}
		}
	}
}

void PCBCollection::update_blocked_time(){
	PCBCollection::pcb_collection->update_blck_time();
}



