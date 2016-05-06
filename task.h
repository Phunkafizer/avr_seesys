/*
	task.h
	Stefan Seegel, post@seegel-systeme.de
	feb 2011
	last update apr 2016
	http://opensource.org/licenses/gpl-license.php GNU Public License
	header file for task class. When a class is derived from Task, the execute
	method will be called automatically.
*/

#ifndef task_h
#define task_h

#include <avr/io.h>

class Task
{
private:
	static Task *last;
	Task *prior;
protected:
	virtual void Execute() = 0;
public:
	Task();
	static void run();
};

#endif
