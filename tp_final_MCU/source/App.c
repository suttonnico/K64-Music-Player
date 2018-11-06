
#include "App.h"
#include "dac_adc.h"
#include "fsl_debug_console.h"
#include "Player.h"
#include "SysTick.h"
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>




/*
int unscramble(uint32_t x)
{
	return (x&0x0000FF-'0')*1000;
}
*/

static int i = 0;
void App_Init (void)
{
	Player_Init();
	Player_Play_Song(0);
	SysTick_Init (2720L,test_play_sample);
	__asm("NOP");

}
void App_Run (void)
{
	//test_play_sample();
	//uint32_t sample_r = 0;
	//uint32_t file_size = 0;
	Player_Fill_Buffer();
	int samples_m = 0;
	//DAC_ADC_Print_ADC_Value();
	__asm("NOP");
	__asm("NOP");

}


