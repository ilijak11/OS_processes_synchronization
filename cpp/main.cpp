/*
 * main.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */


#include "thread.h"
#include "PCB_c.h"
#include "PCB.h"
#include <stdio.h>
#include "SCHEDULE.H"
#include "timer.h"
#include "lock.h"
#include "queue.h"
#include "semaphor.h"
#include "IVTEntry.h"
#include "event.h"


volatile int flag = 0;

class IdleThread: public Thread{
private:
	void run();
	~IdleThread(){
		waitToComplete();
	}
public:
	IdleThread(StackSize ss, Time t): Thread(ss, t){}
};

void IdleThread::run(){
	while(!flag){
		for(int i = 0; i<3000; i++)
			for(int j = 0; j<3000; j++);
	}
}
int userMain(int usr_argc, char* usr_argv[]);
int main(int argc, char* argv[]){
	Timer::setTimer();
	lock;
	PCB* main = new PCB(0,	3, 0);
	Thread* idle = new IdleThread(defaultStackSize, 1);
	idle->start();
	PCB::running = main;
	unlock;
	//char* args[] = { "tekst", "20", "10", "10"};
	int k = userMain(argc, argv);
	printf("User Main returned: %d\n", k);
	lock;
	flag = 1;
	delete idle;
	delete main;
	Timer::resetTimer();
	unlock;
	printf("Well Done!");
	return 0;
}

