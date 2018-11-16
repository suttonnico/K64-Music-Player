/***************************************************************************//**
  @file     Pin.h
  @brief    Simple GPIO Pin services
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _PIN_H_
#define _PIN_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

 typedef enum {

	PIN_CONTROL,

	CANT_PINS
} pinName_t;



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Inicializa el m�dulo
*/
void Pin_Init (void);

/**
 * @brief Escribe un valor en un pin digital
 * @param pin ID del pin
 * @param val Valor digital a escribir
*/
void Pin_Write (pinName_t pin, bool val);

/**
 * @brief Enciende (set) un pin digital
 * @param pin ID del pin
*/
void Pin_Set (pinName_t pin);

/**
 * @brief Apaga (clear) un pin digital
 * @param pin ID del pin
*/
void Pin_Clear (pinName_t pin);

/**
 * @brief Togglea un pin digital
 * @param pin ID del pin
*/
void Pin_Toggle (pinName_t pin);

/**
 * @brief Lee un pin digital
 * @param pin ID del pin
 * @return Valor digital del pin
*/
bool Pin_Read (pinName_t pin);


/*******************************************************************************
 ******************************************************************************/

#endif // _PIN_H_
