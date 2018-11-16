
#include "fsl_dac.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get instance number for DAC module.
 *
 * @param base DAC peripheral base address
 */
static uint32_t DAC_GetInstance(DAC_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to DAC bases for each instance. */
static DAC_Type *const s_dacBases[] = DAC_BASE_PTRS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to DAC clocks for each instance. */
static const clock_ip_name_t s_dacClocks[] = DAC_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Codes
 ******************************************************************************/
static uint32_t DAC_GetInstance(DAC_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_dacBases); instance++)
    {
        if (s_dacBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_dacBases));

    return instance;
}

void DAC_Init(DAC_Type *base,  dac_config_t *config)
{
    assert(NULL != config);

    config->referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
    config->enableLowPowerMode = false;
    uint8_t tmp8;

    /* Enable the clock. */
    //

    CLOCK_EnableClock(s_dacClocks[DAC_GetInstance(base)]);


    /* Configure. */
    /* DACx_C0. */
    tmp8 = base->C0 & ~(DAC_C0_DACRFS_MASK | DAC_C0_LPEN_MASK);
    tmp8 |= DAC_C0_DACRFS_MASK;

    base->C0 = tmp8;

    //enable dac
    base->C0 |= DAC_C0_DACEN_MASK;
}


void DAC_SetBufferValue(DAC_Type *base, uint8_t index, uint16_t value)
{
    assert(index < DAC_DATL_COUNT);

    base->DAT[index].DATL = (uint8_t)(0xFFU & value);         /* Low 8-bit. */
    base->DAT[index].DATH = (uint8_t)((0xF00U & value) >> 8); /* High 4-bit. */
}
