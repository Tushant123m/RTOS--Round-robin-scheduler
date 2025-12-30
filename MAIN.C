#include "stm32f401xe.h"
#include <stdio.h>
#include <math.h>
#include "timebase.h"
#include "uartmy.h"
#include "OsKernel.h"

#define QUANTA 10
typedef uint32_t TASK_PROFILER;
TASK_PROFILER TASK0_PROFILER, TASK1_PROFILER, TASK2_PROFILER ;
void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);


void task0(void){
while(1)
{
	TASK0_PROFILER++;

}
}

void task1(void){
while(1)
{
		TASK1_PROFILER++;
//motor_stop();
}
}

void task2(void){
while(1)
{
		TASK2_PROFILER++;

//valve_open();
}
}
int main(void)
	{
		OsKernelInit();

		OS_KERNEL_ADD_THREADS(&task0, &task1, &task2);
		
		OS_KERNEL_LAUNCH(QUANTA);
		
		while(1){
		
		}
		
}
	
void motor_run(void){
	printf("motor is running");
}

void motor_stop(void){
	printf("motor is stopping");
}

void valve_open(void){
	printf("valve is open");
}

void valve_close(void){
	printf("valve is closing");
}
