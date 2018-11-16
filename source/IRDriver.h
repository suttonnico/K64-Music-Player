
#ifndef IRDRIVER_H_
#define IRDRIVER_H_

#include "Template/LDM.h"

#include "PORT.h"
#include "GPIO.h"
#include "FTM.h"



#define TECLA0 0
#define TECLA1 1
#define TECLA2 2
#define TECLA3 3
#define TECLA4 4
#define TECLA5 5
#define TECLA6 6
#define TECLA7 7
#define TECLA8 8
#define TECLA9 9
#define TECLAVOLMAS 16
#define TECLAVOLMENOS 17



void IC_PISR(void);
void IC_Init (void);
uint8_t getValue(void);
uint8_t getToggle(void);




#endif /* IRDRIVER_H_ */
