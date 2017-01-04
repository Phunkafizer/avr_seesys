# avr_seesys
Timer- and taskclass for 8-bit AVR microcontrollers

#A simple oneshot timer:#
``` c++
#include "timer.h"

void main() {
  sei();
  Timer timer1;
  timer1.SetTime(500);
  
  while (true) {
    if (timer1.IsFlagged()) {
      PORTA ^= 1<<PA0;
      timer1.SetTime(500);
    }
  }
}
```

#A simple continous timer:#
``` c++
#include "timer.h"

void main() {
  sei();
  Timer timer1(500, false);
  
  while (true) {
    if (timer1.IsFlagged())
      PORTA ^= 1<<PA0;
  }
}
```

#Two simple tasks:#
``` c++
#include "task.h"

class Task1: public Task 
protected
  void Execute() {
    //Task 1
  }
};

class Task2: public Task 
protected
  void Execute() {
    //Task 2
  }
};

Task1 task1;
Task2 task2;

void main() {
  while (true) {
    Task::run(); //run all tasks
  }
}
```
