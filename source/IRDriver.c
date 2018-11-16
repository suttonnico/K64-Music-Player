
#include "IRDriver.h"

#include "FTM.h"
#include "PORT.h"
#include "GPIO.h"
#include "Pin.h"
#define IRPIN 18
#define IRPORT PORTB
#define FTMNumber FTM2
#define FTMChannel FTM_CH_0
#define HBT 347
#include "fsl_debug_console.h"

const PCRstr PORT_OUT_PUSHPULL = {0, 0, 0, 0, 0, 0, 1, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0};



enum states {Idle = 0, InProgress = 1};
uint8_t state;
uint8_t bits[15];
uint8_t lastBits[15];



// Primero busco un rising edge, significa 

void IC_PISR (void)
{

	FTM_ClearInterruptFlag( FTMNumber, FTMChannel );

	static uint16_t first = 1, last, result;
	static uint8_t cont = 1;
	static uint16_t first2 = 1, last2, result2;
	static uint8_t firstTime = 1;
	static uint8_t firstHBT = 1;

	GPIO_SetPin(PTC, 3);

	if(firstTime)
	{
		first = FTM_GetCounter( FTMNumber, FTMChannel );
		first2 = first;
		FTM_SetInputCaptureEdge (FTMNumber, FTMChannel,FTM_eRising);
		firstTime = 0;
		firstHBT = 1;
		bits[0] = 1;
		cont = 0;
	}
	else
	{
		last = FTM_GetCounter( FTMNumber, FTMChannel );
		result = last - first;
		if (result > 10500)
		{
			FTM_SetInputCaptureEdge (FTMNumber, FTMChannel,FTM_eFalling);
			firstTime = 1;

			result2 = last - first2 ;
			first2 = last;
			cont++;
			if ( result2 >= 2*HBT && result2 < 3*HBT )
			{
				if ( bits[cont-1] )
					bits[cont]=1;
				else
					bits[cont]=0;
			}
			if ( result2 >= 3*HBT && result2 < 4*HBT )
			{
				if ( bits[cont-1] )
				{
					bits[cont]=1;
					cont++;
					bits[cont]=0;
				}
				else
					bits[cont]=1;
			}
			if ( result2 >= 4*HBT )
			{
				bits[cont]=1;
				cont++;
				bits[cont]=0;
			}


			if ( cont >= 14 )
			{
				for ( int i = 0; i < 15; i++)
				{
					lastBits[i] = bits[i];
					bits[i] = 0;
					cont = 0;

				}
				PRINTF("%d\n",getValue());
			}

		}
		else
		{
			result2 = last - first2 + firstHBT * HBT;
			firstHBT = 0;
			first2 = last;
			cont++;
			if ( cont == 13){
				__asm("NOP");
			}
			if ( result2 >= 2*HBT && result2 < 3*HBT )
			{
				if ( bits[cont-1] )
					bits[cont]=1;
				else
					bits[cont]=0;
			}
			if ( result2 >= 3*HBT && result2 < 4*HBT )
			{
				if ( bits[cont-1] )
				{
					bits[cont]=1;
					cont++;
					bits[cont]=0;
				}
				else
					bits[cont]=1;
			}
			if ( result2 >= 4*HBT )
			{
				bits[cont]=1;
				cont++;
				bits[cont]=0;
			}
		}



	}
	GPIO_ClearPin(PTC, 3);
}


void IC_Init (void){
	PORT_Init();
	PORT_Configure2(PORTC, 3, PORT_OUT_PUSHPULL);
	GPIO_SetDirPin(PTC, 3, GPIO__OUT);
	GPIO_SetPin(PTC, 3);

	PCRstr UserPCR;
	UserPCR.PCR=false;													// Todo apagado por default, prendo lo necesario
	UserPCR.FIELD.DSE=true;
	UserPCR.FIELD.MUX=PORT_mAlt3;
	UserPCR.FIELD.IRQC=PORT_eDisabled;

	PORT_Configure2 (IRPORT,IRPIN,UserPCR);

	FTMNumber -> FILTER = 0X00000FFF;
	FTM_SetPrescaler(FTMNumber, FTM_PSC_x128);	  						// BusClock=sysclk/2= 50MHz y prescaler = divx128
	FTMNumber->CNTIN=0x0000;				  							/// Free running
	FTMNumber->MOD=0xFFFF;
	FTM_SetWorkingMode(FTMNumber,FTMChannel,FTM_mInputCapture);  		// IC Function and toggle output
	FTM_SetInputCaptureEdge (FTMNumber, FTMChannel,FTM_eFalling); 		// Seteo Falling Edge para recibir el primer cambio del bit de start
	FTM_SetInterruptMode (FTMNumber,FTMChannel, true); 					// Habilito interrupciones
	FTM_StartClock(FTMNumber); 											// Selecciono BusClk

	state = Idle;

}

uint8_t getValue(void){

	return lastBits[9]*32 + lastBits[10]*16 + lastBits[11]*8 + lastBits[12]*4 + lastBits[13]*2 +lastBits[14];

}




uint8_t getToggle(void){

	return lastBits[3];

}


