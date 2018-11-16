
/***************************************************************************//**
  @file     Pin.c
  @brief    Simple GPIO Pin services
  @author   Nicol�s Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Pin.h"
#include "PORT.h"
#include "GPIO.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define PIN_PORT_SIMPLE		{0, 0, 0, 0, 0, 0, true, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0}
#define PIN_OUT_PUSHPULL	PIN_PORT_SIMPLE

#define PIN_IN_PULLUP		{1, 1, 0, 0, 0, 0, false, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0}
#define PIN_IN_PULLDOWN		{0, 1, 0, 0, 0, 0, false, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0}
#define PIN_IN_NOPULL		{0, 0, 0, 0, 0, 0, false, 0, PORT_mGPIO, 0, 0, PORT_eDisabled, 0, 0, 0}

//

#define PIN_PWM				{0, 0, 0, 0, 0, 0, true, 0, PORT_mAlt4, 0, 0, PORT_eDisabled, 0, 0, 0}

#define PIN_DMA				{1, 1, 0, 0, 0, 0, true, 0, PORT_mAlt4, 0, 0, PORT_eDMAFalling, 0, 0, 0}			// FTM0_CH1
#define PIN_IR_CONFIG		{0, 0, 0, 0, 0, 0, false, 0, PORT_mGPIO, 0, 0, PORT_eInterruptFalling, 0, 0, 0}		// IR

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
	PORT_t port;
	GPIO_t gpio;
	uint32_t num;
	PCRstr portConfig;
	GPIODirection_t dir;
	GPIOData_t onLevel;
	GPIOData_t initLevel;
} pinConfig_t;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static const pinConfig_t pinList[CANT_PINS] = {
		//    port,  gpio, num, portConfig,      dir,       onLevel,  initLevel

		/**********************************************************/


		{ PORTC,  PTC,    3, 	PIN_PORT_SIMPLE, GPIO__OUT, GPIO__HI, GPIO__LO }

};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Pin_Init (void)
{
	const pinConfig_t* pin;
	static uint8_t yaInit = false;
	if (yaInit)
		return;

	PORT_Init();
	GPIO_Init();

	for (pin=pinList ; pin<pinList+CANT_PINS ; ++pin)
	{
		PORT_Configure2(pin->port, pin->num, pin->portConfig);

		if (pin->portConfig.FIELD.MUX == PORT_mGPIO){
			GPIO_SetDirection(pin->gpio, pin->num, pin->dir);
			if (pin->dir == GPIO__OUT)
			{
				GPIO_Write(pin->gpio, pin->num, pin->initLevel);
			}
		}
		else if (pin->portConfig.FIELD.MUX == PORT_mAlt2){
			//inicializacion aca?
		}
	}
}


void Pin_Write (pinName_t pin, bool val)
{
	if (pin<CANT_PINS && pinList[pin].dir==GPIO__OUT)
	{
		GPIO_Write(pinList[pin].gpio, pinList[pin].num, !val == !(pinList[pin].onLevel));
	}
}

void Pin_Set (pinName_t pin)
{
	if (pin<CANT_PINS && pinList[pin].dir==GPIO__OUT)
	{
		if (pinList[pin].onLevel)
			GPIO_Set(pinList[pin].gpio, pinList[pin].num);
		else
			GPIO_Clear(pinList[pin].gpio, pinList[pin].num);
	}
}

void Pin_Clear (pinName_t pin)
{
	if (pin<CANT_PINS && pinList[pin].dir==GPIO__OUT)
	{
		if (pinList[pin].onLevel)
			GPIO_Clear(pinList[pin].gpio, pinList[pin].num);
		else
			GPIO_Set(pinList[pin].gpio, pinList[pin].num);
	}
}

void Pin_Toggle (pinName_t pin)
{
	if (pin<CANT_PINS && pinList[pin].dir==GPIO__OUT)
	{
		;
//		GPIO_Toggle(pinList[pin].gpio, pinList[pin].num);
	}
}

bool Pin_Read (pinName_t pin)
{
	bool rta = false;
	if (pin<CANT_PINS)
	{
		rta = !(pinList[pin].onLevel) == !(GPIO_Read(pinList[pin].gpio, pinList[pin].num));
	}
	return rta;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

