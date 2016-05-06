/*
 * timedtask.h
 *
 * Created: 04.05.2016 15:58:37
 *  Author: sseegel
 */ 

#include "task.h"
#include "timer.h"

#ifndef TIMEDTASK_H_
#define TIMEDTASK_H_

class TimedTask: public Task {
private:
	Timer timer;
	void Execute();
protected:
	virtual void onTask() = 0;
public:
	TimedTask(uint16_t ticks);
};

#endif /* TIMEDTASK_H_ */