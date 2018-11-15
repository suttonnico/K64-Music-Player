
#include "IRDriver.h"

#include "FTM.h"
#include "PORT.h"
#include "GPIO.h"

#define IRPIN 9
#define IRPORT PORTC
#define FTMNumber FTM3
#define FTMChannel FTM_CH_5
#define HBT 347





enum states {Idle = 0, InProgress = 1};
uint8_t state;
uint8_t bits[15];
uint8_t lastBits[15];



// Primero busco un rising edge, significa 

void IC_PISR (void){

	static uint16_t first, last, result;
	static uint8_t cont = 1;
	static uint8_t firstTime = 1;

	FTM_ClearInterruptFlag( FTMNumber, FTMChannel );					// Limpio los flags de interrupcion

	if( state = Idle ){
		first = FTM_GetCounter( FTMNumber, FTMChannel ); 				// Guardo el valor del FTM 
		FTM_SetInputCaptureEdge (FTMNumber, FTMChannel,FTM_eRising);	// Cambio el flanco que busco
		bits[0] = 1;													// Agrego 1 ficticio para poder decodificar
		cont = 1;
		firstTime = 1;													// Sumar un HBT
		state = InProgress;												// Cambio a estado en progreso
	}
	else if( state == InProgress ){
		cont++;
		last = FTM_GetCounter( FTMNumber, FTMChannel );					// Saco el valor actual
		result = last - first + HBT * firstTime;						// Obtengo cantidad de tiempo entre flancos ^
		first = last;													// Reseteo el contador 
		firstTime = 0;
		if ( result >= 2*HBT && result < 3*HBT ){
			if ( bits[cont-1] )
				bits[cont]=1;
			else
				bits[cont]=0;
		}
		if ( result >= 3*HBT && result < 4*HBT ){
			if ( bits[cont-1] ){
				bits[cont]=1;
				cont++;
				bits[cont]=0;
			}
			else
				bits[cont]=1;
		}
		if ( result >= 4*HBT ){
			bits[cont]=1;
			cont++;
			bits[cont]=0;
		}

	}

	if ( cont > 14 ){
		state = Idle;
		FTM_SetInputCaptureEdge (FTMNumber, FTMChannel,FTM_eFalling);
		for ( int i = 0; i < 15; i++)
			lastBits[i] = bits[i];
	}

}


void IC_Init (void){
	PCRstr UserPCR;
	UserPCR.PCR=false;													// Todo apagado por default, prendo lo necesario
	UserPCR.FIELD.DSE=true;
	UserPCR.FIELD.MUX=PORT_mAlt3;
	UserPCR.FIELD.IRQC=PORT_eDisabled;

	PORT_Configure2 (IRPORT,IRPIN,UserPCR);

	FTM_SetPrescaler(FTMNumber, FTM_PSC_x128);	  						// BusClock=sysclk/2= 50MHz y presccaler = divx32
	FTMNumber->CNTIN=0x0000;				  							/// Free running
	FTMNumber->MOD=0xFFFF;
	FTM_SetWorkingMode(FTMNumber,FTMChannel,FTM_mInputCapture);  		// OC Function and toggle output
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


