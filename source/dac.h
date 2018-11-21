/*
 * dac.h
 *
 *  Created on: 13/2/2018
 *      Author: Nicolas
 */

#ifndef DAC_H_
#define DAC_H_


#include "fsl_common.h"

typedef enum _dac_reference_voltage_source
{
    kDAC_ReferenceVoltageSourceVref1 = 0U, /*!< The DAC selects DACREF_1 as the reference voltage. */
    kDAC_ReferenceVoltageSourceVref2 = 1U, /*!< The DAC selects DACREF_2 as the reference voltage. */
} dac_reference_voltage_source_t;

typedef struct _dac_config
{
    dac_reference_voltage_source_t referenceVoltageSource; /*!< Select the DAC reference voltage source. */
    bool enableLowPowerMode;                               /*!< Enable the low-power mode. */
} dac_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

void DAC_Init(DAC_Type *base, dac_config_t *config);
void DAC_Deinit(DAC_Type *base);
void DAC_GetDefaultConfig(dac_config_t *config);



void DAC_Set_Value(uint16_t value);

#if defined(__cplusplus)
}
#endif
/*!
 * @}
 */


#endif /* DAC_H_ */
