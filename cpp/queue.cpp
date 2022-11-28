/*
 * queue.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */
#include "queue.h"
#include "lock.h"
#include <stdio.h>



Queue::Queue(): head(0), last(0), size(0){}

Queue::~Queue(){
	this->q_delete();
}

void Queue::put(PCB* pcb){
	QNode* tmp = new QNode(pcb);
	//if(tmp!=0) printf("Put Thr: %d in queue\n", tmp->pcb->id);
	//else printf("ERROR\n");
	//this->last = (!this->head)?this->head:this->last->next = tmp;
	if(this->head == 0) this->head = tmp;
	else this->last->next = tmp;
	this->last = tmp;
	this->size++;
}

PCB* Queue::get(){
	QNode* tmp = 0;
	if(this->head != 0) tmp = this->head;
	else return 0;
	if(this->head->next != 0) this->head = this->head->next;
	else this->last = this->head = 0;
	PCB* pcb = tmp->pcb;
	delete tmp;
	this->size--;
	//printf("Got Thr: %d form queue\n", pcb->id);
	return pcb;
}

void Queue::q_delete(){
	QNode* old = 0;
	while(this->head != 0){
		old = this->head;
		this->head = this->head->next;
		delete old;
	}
	this->head = this->last = 0;
	this->size = 0;
}

void Queue::qprint(){
	for(QNode* curr = head; curr!=0; curr = curr->next){
		curr->pcb->print();
	}
	putchar('\n');
}

int Queue::find_and_remove(PCB* pcb){
	QNode* curr = head;
	QNode* prev = 0;
	for(curr = head; curr != 0; prev = curr, curr = curr->next){
		if(curr->pcb == pcb) break;
	}
	if(curr == 0) return 0;
	if(curr == head){
		head = head->next;
		delete curr;
	}
	else{
		prev->next = curr->next;
		if(curr->next == 0) last = prev;
		delete curr;
	}
	return 1;
}

int Queue::is_empty(){
	return (head == 0)?1:0;
}

