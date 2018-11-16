
#include "dac_adc.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_adc16.h"
#include "fsl_dac.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DAC_BASEADDR DAC0


#define VREF_BRD 3.300
#define SE_12BIT 4096.0




void DAC_ADC_Init(void)
{

    dac_config_t dacConfigStruct;

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    //DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DAC_BASEADDR, &dacConfigStruct);
    //DAC_Enable(DAC_BASEADDR, true); /* Enable output. */


}



void test_set_DAC(int value)
{
	DAC_SetBufferValue(DAC_BASEADDR, 0U, (uint16_t)value);
}
