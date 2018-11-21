/*
 * SysTick.c
 *
 *  Created on: Oct 16, 2018
 *      Author: nico
 */


#include "SysTick.h"


//DEBUG
#include "PORT.h"
#include "GPIO.H"
const PCRstr PORT_OUT_PUSHPULL3 = {0, 0, 0, 0, 0, 0, 1, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0};

static void (*dispatch_pisr_ptr)(void);



void SysTick_Init (int time,void* dispatcher_ptr)
{
	//DEBUG
	PORT_Init();
	PORT_Configure2(PORTC, 5, PORT_OUT_PUSHPULL3);
	GPIO_SetDirPin(PTC, 5, GPIO__OUT);
	GPIO_SetPin(PTC, 5);



	dispatch_pisr_ptr = dispatcher_ptr;
	SysTick->CTRL = 0x00;
	SysTick->LOAD = time-1;//25000L-1; // <= 250 us
	SysTick->VAL  = 0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}




__ISR__  SysTick_Handler (void)
{

	//DEBUG
			GPIO_SetPin(PTC, 5);

	(*dispatch_pisr_ptr)();
	//DEBUG
	GPIO_ClearPin(PTC, 5);

}
