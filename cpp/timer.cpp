/*
 * timer.cpp
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */
#include "timer.h"
#include <stdio.h>
#include "lock.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "PCB_c.h"

Timer* Timer::timer = 0;

volatile unsigned oldHSeg;
volatile unsigned oldHOff;
volatile unsigned t_ss;
volatile unsigned t_sp;
volatile unsigned t_bp;

volatile int time_elapsed = 0;

void tick();

void interrupt timerHandler(){
	if(!on_demand){
		tick();
		asm int 60h
		PCBCollection::update_blocked_time();
		time_elapsed = PCB::running->elapsed();
	}
	if(on_demand || time_elapsed == 1){
		//printf("context switch - on_demand = %d\n", on_demand);
		//asm cli;
		asm {
			mov t_ss, ss
			mov t_sp, sp
			mov t_bp, bp
		}

	//printf("old running: ID: %d  stack seg: %x stack off %x\n", PCB::running->id, t_ss, t_sp);
		//asm cli;

		PCB::running->ss = t_ss;
		PCB::running->sp = t_sp;
		PCB::running->bp = t_bp;


		if(PCB::running->finished == 0 && PCB::running->blocked == 0
				&& PCB::running->waiting_to_complete  == 0 && PCB::running->event_blocked == 0){
			Scheduler::put(PCB::running);
		//printf("Put Thread: %d\n", PCB::running->id);
			//asm cli;
		}
		PCB::running = Scheduler::get();
		PCB::running->elapsed();

		t_ss = PCB::running->ss;
		t_sp = PCB::running->sp;
		t_bp = PCB::running->bp;

	//printf("new running: ID: %d  stack seg: %x stack off %x\n", PCB::running->id, t_ss, t_sp);
		//asm cli;

		asm{
			mov ss, t_ss
			mov sp, t_sp
			mov bp, t_bp
		}
		on_demand = 0;
	}
}


Timer::Timer(){
	//treba u ivtp staviti novi handler
	printf("Setting new timer handler\n");
	asm {
		cli
		push ax
		push es

		mov ax, 0
		mov es, ax

		//ulaz za timer na ulazu 8h >>

		mov ax, word ptr es:20h
		mov oldHOff, ax
		mov ax, word ptr es:22h
		mov oldHSeg, ax
		//sacuvana stara rutina

		mov word ptr es:20h, offset timerHandler
		mov word ptr es:22h, seg timerHandler
		// postavljena nova

		mov ax, oldHOff
		mov word ptr es:180h, ax
		mov ax, oldHSeg
		mov word ptr es:182h, ax
		// stara rutina na int 60

		pop es
		pop ax

		sti
	}
}

Timer::~Timer(){
	asm {
		//vracanje stare rutine
		cli

		push ax
		push es

		mov ax, oldHOff
		mov word ptr es:20h, ax
		mov ax, oldHSeg
		mov word ptr es:22h, ax

		pop es
		pop ax

		sti
	}
	printf("Restored old timer handler\n");
}


void Timer::setTimer(){
	Timer::timer = new Timer();
}

void Timer::resetTimer(){
	delete Timer::timer;
}


