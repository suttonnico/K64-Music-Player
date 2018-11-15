
#include "App.h"
#include "dac_adc.h"
#include "fsl_debug_console.h"
#include "Player.h"
#include "SysTick.h"
//#include "actions.h"
#include "IRDriver.h"
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
	//PRINTF("RUN\n");
	Player_Fill_Buffer();
	if (getToggle()==1)
			{
				switch (0)//getValue())
				{
				case 0:
					PRINTF("Pause\n");//Play/Pausa
					Player_Play();
				break;
				case '1':							//Poner Cancion1
					Player_Play_Song(0);
				break;
				case '2':							//Poner Cancion2
					Player_Play_Song(1);
				break;
				case '3':							//Poner Cancion3
					//
				break;
				case '4':							//Poner Cancion4
					//
				break;
				case '5':							//Poner Cancion5
					//
				break;
				case '16':							//Subir el Volumen
					Player_Up_Volume();
				break;
				case '17':							//Bajar el Volumen
					Player_Down_Volume();
				break;

				}
			}

	int samples_m = 0;
	//DAC_ADC_Print_ADC_Value();

}


