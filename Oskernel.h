#ifndef __OSKERNEL_H
#define __OSKERNEL_H

#include "stm32f401xe.h"
#include <stdint.h>

	void OS_SCHEDULER_LAUNCH(void);
	uint8_t OS_KERNEL_ADD_THREADS(void(*task0)(void), void(*task1)(void),void(*task2)(void));
 void OS_SCHEDULER_LAUNCH(void);
	void OS_KERNEL_LAUNCH(int quanta);
	void OsKernelInit(void);
#endif
