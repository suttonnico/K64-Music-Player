/*
 * SysTick.c
 *
 *  Created on: Oct 16, 2018
 *      Author: nico
 */


#include "SysTick.h"




static void (*dispatch_pisr_ptr)(void);



void SysTick_Init (int time,void* dispatcher_ptr)
{
	dispatch_pisr_ptr = dispatcher_ptr;
	SysTick->CTRL = 0x00;
	SysTick->LOAD = time-1;//25000L-1; // <= 250 us
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}




__ISR__  SysTick_Handler (void)
{

	(*dispatch_pisr_ptr)();

}
