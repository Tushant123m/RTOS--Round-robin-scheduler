README:

This Round Robin scheduler is executing 3 tasks, each in a specific time quanta of 10ms, and when 1 task time is finished, all its content of registers is transfered to dedicated stack, and from task2 stack, content is preloaded into R0-R12, and task2 runs. 

Next, task2 halts, and task 3 runs in the similar manner. Dedicated stacks are allocated for each tasks, from where data is laoded and unloaded in cycles.
All the files are Os kernal.c, Oskernal.h, and uart files are given for implementing this project. Software used was keil uvision.
UART was used for printf statements execution by the STM32f401re board to te serial monitor of our computer. 

Systick timer interupt is used for halting tasks after each quanta, after each 10ms time, systick generates an interupt, then the contents of current task( all R0-R12, and xPR, PC, everything), is loaded into that currrent task's dedicated stack, and next task in queue content is preloaded from its dedicated stack to our PC, SP, R0-R12.
Configure Systick, UART, and GPIO for doing this project.

STM32 was prefered in this project, as it helps in working out with regiters R0-R12, PC, SP directly with help of debugging tools(JTAG), rather than arduino or esp32 boards.
