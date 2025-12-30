#include "OsKernel.h"

#define ONE_MS_SEC_LOAD	16000 

#define CTRL_ENABLE    (1U<<0)
#define CTRL_TICKINT   (1U<<1)
#define CTRL_CLCKSRC   (1U<<2)
#define CTRL_COUNTFLAG (1U<<16)

#define NUM_OF_THTREADS 3
#define STACKSIZE 			400
#define BUS_FREQ				16000000
uint32_t MILLIS_PRESCALER;
 struct tcb{
 int32_t *sp;
 struct tcb *next;
};
typedef struct tcb tcbtype;
tcbtype tcbs[NUM_OF_THTREADS];
tcbtype *CurrentPt;

uint32_t tcb_stack[NUM_OF_THTREADS][STACKSIZE];


void OS_KERNEL_TCB_STACK_INIT(int i){

tcbs[i].sp = &tcb_stack[i][STACKSIZE - 16];

	tcb_stack[i][STACKSIZE - 1] = (1U<<24); //PSR in thumb mode
																				//skip PC  as it has to be intialized with task(i) starting address
	tcb_stack[i][STACKSIZE - 3] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 4] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 5] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 6] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 7] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 8] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 9] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 10] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 11] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 12] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 13] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 14] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 15] = 0xAAAAAAAA;
	tcb_stack[i][STACKSIZE - 16] = 0xAAAAAAAA;
}

uint8_t OS_KERNEL_ADD_THREADS(void(*task0)(void), void(*task1)(void),void(*task2)(void))
{
	
	__disable_irq();
 tcbs[0].next = &tcbs[1];
	tcbs[1].next = &tcbs[2];
	tcbs[2].next = &tcbs[0];
	
	/*initalize PC of all to task(i) starting address*/
	OS_KERNEL_TCB_STACK_INIT(0);
	tcb_stack[0][STACKSIZE - 2] =  (int32_t)(task0);
	OS_KERNEL_TCB_STACK_INIT(1);
	tcb_stack[1][STACKSIZE - 2] =  (int32_t)(task1);
	OS_KERNEL_TCB_STACK_INIT(2);
	tcb_stack[2][STACKSIZE - 2] =  (int32_t)(task2);
	
	CurrentPt = &tcbs[0];
	__enable_irq();
	return 1;
}

		
void OsKernelInit(void){
MILLIS_PRESCALER = (BUS_FREQ/1000);
}
				

void OS_KERNEL_LAUNCH(int quanta){
	
	//reset the systick
	SysTick->CTRL = 0;
	
		/*Clear systick current value register*/
		SysTick->VAL = 0;

		/*Reload the timer with the number of cycles per second*/
		SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;
	
		NVIC_SetPriority(SysTick_IRQn,15);/*set to low priority */

	/*Select internal clock source*/
		SysTick->CTRL = CTRL_CLCKSRC | CTRL_ENABLE;

	/*enable interupt*/
		SysTick->CTRL |= CTRL_TICKINT;
	
	OS_SCHEDULER_LAUNCH();
	
}


void SysTick_Handler(void) {
    __asm volatile (
        "CPSID I            \n\t"  // Disable interrupts
        "PUSH {R4, R11}     \n\t"  // Save registers R4 and R11 on the stack

        "LDR R0, =CurrentPt \n\t"  // Load the address of CurrentPt into R0
        "LDR R1, [R0]       \n\t"  // Dereference CurrentPt, load value into R1
        "STR SP, [R1]       \n\t"  // Store the current SP (stack pointer) at [R1]

        "LDR R1, [R1, #4]   \n\t"  // Load the next task pointer into R1
        "STR R1, [R0]       \n\t"  // Update CurrentPt to point to the next task
        "LDR SP, [R1]       \n\t"  // Load the stack pointer for the new task into SP

        "POP {R4, R11}      \n\t"  // Restore registers R4 and R11
        "CPSIE I            \n\t"  // Enable interrupts
        "BX LR              \n\t"  // Return from the exception handler
    );
}

	
void OS_SCHEDULER_LAUNCH(void) {
    __asm volatile (
       
        "LDR R0, =CurrentPt \n\t"  // Load the address of CurrentPt into R0
        "LDR R2, [R0]       \n\t"  // Dereference CurrentPt, load the current task pointer into R2
        "LDR SP, [R2]       \n\t"  // Load the saved stack pointer of the current task into SP
        
        "POP {R4, R11}      \n\t"  // Restore registers R4 and R11
        "POP {R12}          \n\t"  // Restore register R12
        "POP {R0, R3}       \n\t"  // Restore registers R0 and R3
        "ADD SP, SP, #4     \n\t"  // Skip one word (to skip PSR)
        "POP {LR}           \n\t"  // Restore LR (Link Register)
        "ADD SP, SP, #4     \n\t"  // Skip one word (to reach PC)
        
        "CPSIE I            \n\t"  // Enable interrupts
        "BX LR              \n\t"  // Branch to the address in LR (start task execution)
    );
}






