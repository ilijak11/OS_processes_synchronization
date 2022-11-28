/*
 * timer.h
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer{
public:
	static Timer* timer;
	static void setTimer();
	static void resetTimer();
	Timer();
	~Timer();
};

extern volatile int on_demand;
void interrupt timerHandler();
void tick();



#endif /* TIMER_H_ */
