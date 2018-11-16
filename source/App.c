
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
#include "Pin.h"



/*
int unscramble(uint32_t x)
{
	return (x&0x0000FF-'0')*1000;
}
*/


static int i = 0;
void App_Init (void)
{
	FTM_Init();
	IC_Init();




//	Pin_Init();

	Player_Init();
	Player_Play_Song(0);
	SysTick_Init (2720L,test_play_sample);


}
void App_Run (void)
{


//	PRINTF("%d\n",getValue());
	Player_Fill_Buffer();
	static uint8_t prevTog = 1;
	uint8_t curTog;
	curTog = getToggle();
	if (curTog != prevTog)

			{
		prevTog = curTog;
				switch (getValue())
				{
				case 12:
//					PRINTF("Pause\n");//Play/Pausa
					Player_Play();
				break;
				case  1:							//Poner Cancion1
					Player_Play_Song(0);
				break;
				case 2:							//Poner Cancion2
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
				case 16:							//Subir el Volumen
					Player_Up_Volume();
				break;
				case 17:							//Bajar el Volumen
					Player_Down_Volume();
				break;

				}
			}

	int samples_m = 0;
	//DAC_ADC_Print_ADC_Value();

}


