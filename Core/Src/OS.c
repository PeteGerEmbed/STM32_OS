
#include <stdint.h>
#include "OS.h"
#include "main.h"


int8_t thread_counter = 0;
TCB *current_thread_pt;
TCB tcb[MAX_THREADS] = {0};


void halt_us(uint32_t time)
{
	for (uint32_t i = 0; i < time; ++i) {
		NOP_US;
	}

}

void halt_ms(uint32_t time)
{
	for(uint32_t i = 0; i < time; ++i) {
		for (uint32_t ii = 0; ii < 64; ++ii) {
			NOP_MS;
		}
	}
}


void create_thread(void (*thread_handler)(void))
{
	if(thread_ctr < MAX_THREADS)
	{

		tcb[thread_ctr].stack_pointer = &tcb[thread_ctr].stack[STACK_SIZE];

		--tcb[thread_ctr].stack_pointer; *(tcb[thread_ctr].stack_pointer) |= 1<<24;						/*xPSR*/
		*(--tcb[thread_ctr].stack_pointer) = (uint32_t) thread_handler;									/*PC*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF; 												/* LR*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R12*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R3*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R2*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R1*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;   												/*R0*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R11*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R10*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R9*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R8*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;   												/*R7*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R6*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R5*/
		*(--tcb[thread_ctr].stack_pointer) = 0xDEADBEEF;												/*R4*/

		/* determined values for stack overflow detection */
		for (i = 0; i < 9; ++i) {
			tcb[thread_counter].stack[i] = 0xDEADFACE;
		}

		++thread_ctr;
	}

}

void os_init(void)
{
	SysTick_Config(SystemCoreClock/1000);

	/*Linking the elements of the list*/

	if (thread_counter >= 2) {
		for (i = 0; i < thread_counter-1; ++i)	tcb[i].next = &tcb[i+1];
		tcb[thread_counter-1].next = &tcb[0];
	}
	else if(thread_counter == 1) tcb[0].next = &tcb[0];
	else if(thread_counter == 0) /* Call fault handler function */




	/*Choosing the first thread*/

	current_thread_pt = &tcb[0];


	__asm("LDR	R0,=current_thread_pt");			//R0 = &current_thread_pt
	__asm("LDR	R1, [R0]");							//R1 = current_thread_pt
	__asm("LDR	SP, [R1]");							//SP = *current_thread_pt
	__asm("pop	{r0-r12}");
	__asm("add	sp,sp,#4");							//Skip LR register
	__asm("POP	{LR}");								//Load thread address into PC
	__asm("add	sp,sp,#4");							//Skip xPSR register
	__asm("CPSIE	I");
	__asm("bx 	lr");								//Return to 1st thread
}

void system_fault_handler(void)
{
	__disable_irq();

	GPIOA->ODR ^= 1<<5;
	HAL_Delay(500);

}








