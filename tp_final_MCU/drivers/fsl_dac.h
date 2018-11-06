
#ifndef _FSL_DAC_H_
#define _FSL_DAC_H_

#include "fsl_common.h"

/*!
 * @addtogroup dac
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief DAC driver version 2.0.1. */
#define FSL_DAC_DRIVER_VERSION (MAKE_VERSION(2, 0, 1))
/*@}*/

/*!
 * @brief DAC buffer flags.
 */
enum _dac_buffer_status_flags
{
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION) && FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    kDAC_BufferWatermarkFlag = DAC_SR_DACBFWMF_MASK,                  /*!< DAC Buffer Watermark Flag. */
#endif                                                                /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    kDAC_BufferReadPointerTopPositionFlag = DAC_SR_DACBFRPTF_MASK,    /*!< DAC Buffer Read Pointer Top Position Flag. */
    kDAC_BufferReadPointerBottomPositionFlag = DAC_SR_DACBFRPBF_MASK, /*!< DAC Buffer Read Pointer Bottom Position
                                                                           Flag. */
};

/*!
 * @brief DAC buffer interrupts.
 */
enum _dac_buffer_interrupt_enable
{
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION) && FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION
    kDAC_BufferWatermarkInterruptEnable = DAC_C0_DACBWIEN_MASK,         /*!< DAC Buffer Watermark Interrupt Enable. */
#endif                                                                  /* FSL_FEATURE_DAC_HAS_WATERMARK_DETECTION */
    kDAC_BufferReadPointerTopInterruptEnable = DAC_C0_DACBTIEN_MASK,    /*!< DAC Buffer Read Pointer Top Flag Interrupt
                                                                             Enable. */
    kDAC_BufferReadPointerBottomInterruptEnable = DAC_C0_DACBBIEN_MASK, /*!< DAC Buffer Read Pointer Bottom Flag
                                                                             Interrupt Enable */
};

/*!
 * @brief DAC reference voltage source.
 */
typedef enum _dac_reference_voltage_source
{
    kDAC_ReferenceVoltageSourceVref1 = 0U, /*!< The DAC selects DACREF_1 as the reference voltage. */
    kDAC_ReferenceVoltageSourceVref2 = 1U, /*!< The DAC selects DACREF_2 as the reference voltage. */
} dac_reference_voltage_source_t;

/*!
 * @brief DAC buffer trigger mode.
 */
typedef enum _dac_buffer_trigger_mode
{
    kDAC_BufferTriggerByHardwareMode = 0U, /*!< The DAC hardware trigger is selected. */
    kDAC_BufferTriggerBySoftwareMode = 1U, /*!< The DAC software trigger is selected. */
} dac_buffer_trigger_mode_t;

#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION) && FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION
/*!
 * @brief DAC buffer watermark.
 */
typedef enum _dac_buffer_watermark
{
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD) && FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD
    kDAC_BufferWatermark1Word = 0U, /*!< 1 word  away from the upper limit. */
#endif                              /* FSL_FEATURE_DAC_HAS_WATERMARK_1_WORD */
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS) && FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS
    kDAC_BufferWatermark2Word = 1U, /*!< 2 words away from the upper limit. */
#endif                              /* FSL_FEATURE_DAC_HAS_WATERMARK_2_WORDS */
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS) && FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS
    kDAC_BufferWatermark3Word = 2U, /*!< 3 words away from the upper limit. */
#endif                              /* FSL_FEATURE_DAC_HAS_WATERMARK_3_WORDS */
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS) && FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS
    kDAC_BufferWatermark4Word = 3U, /*!< 4 words away from the upper limit. */
#endif                              /* FSL_FEATURE_DAC_HAS_WATERMARK_4_WORDS */
} dac_buffer_watermark_t;
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION */

/*!
 * @brief DAC buffer work mode.
 */
typedef enum _dac_buffer_work_mode
{
    kDAC_BufferWorkAsNormalMode = 0U, /*!< Normal mode. */
#if defined(FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE) && FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE
    kDAC_BufferWorkAsSwingMode,       /*!< Swing mode. */
#endif                                /* FSL_FEATURE_DAC_HAS_BUFFER_SWING_MODE */
    kDAC_BufferWorkAsOneTimeScanMode, /*!< One-Time Scan mode. */
#if defined(FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE) && FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE
    kDAC_BufferWorkAsFIFOMode, /*!< FIFO mode. */
#endif                         /* FSL_FEATURE_DAC_HAS_BUFFER_FIFO_MODE */
} dac_buffer_work_mode_t;

/*!
 * @brief DAC module configuration.
 */
typedef struct _dac_config
{
    dac_reference_voltage_source_t referenceVoltageSource; /*!< Select the DAC reference voltage source. */
    bool enableLowPowerMode;                               /*!< Enable the low-power mode. */
} dac_config_t;

/*!
 * @brief DAC buffer configuration.
 */
typedef struct _dac_buffer_config
{
    dac_buffer_trigger_mode_t triggerMode; /*!< Select the buffer's trigger mode. */
#if defined(FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION) && FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION
    dac_buffer_watermark_t watermark; /*!< Select the buffer's watermark. */
#endif                                /* FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION */
    dac_buffer_work_mode_t workMode;  /*!< Select the buffer's work mode. */
    uint8_t upperLimit;               /*!< Set the upper limit for the buffer index.
                                           Normally, 0-15 is available for a buffer with 16 items. */
} dac_buffer_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Initializes the DAC module.
 *
 * This function initializes the DAC module including the following operations.
 *  - Enabling the clock for DAC module.
 *  - Configuring the DAC converter with a user configuration.
 *  - Enabling the DAC module.
 *
 * @param base DAC peripheral base address.
 * @param config Pointer to the configuration structure. See "dac_config_t".
 */
void DAC_Init(DAC_Type *base, dac_config_t *config);
void DAC_Deinit(DAC_Type *base);
void DAC_GetDefaultConfig(dac_config_t *config);

/*
static inline void DAC_Enable(DAC_Type *base, bool enable)
{
    if (enable)
    {
        base->C0 |= DAC_C0_DACEN_MASK;
    }
    else
    {
        base->C0 &= ~DAC_C0_DACEN_MASK;
    }
}
*/

static inline void DAC_EnableBuffer(DAC_Type *base, bool enable)
{
    if (enable)
    {
        base->C1 |= DAC_C1_DACBFEN_MASK;
    }
    else
    {
        base->C1 &= ~DAC_C1_DACBFEN_MASK;
    }
}
void DAC_SetBufferConfig(DAC_Type *base, const dac_buffer_config_t *config);
void DAC_GetDefaultBufferConfig(dac_buffer_config_t *config);
static inline void DAC_EnableBufferDMA(DAC_Type *base, bool enable)
{
    if (enable)
    {
        base->C1 |= DAC_C1_DMAEN_MASK;
    }
    else
    {
        base->C1 &= ~DAC_C1_DMAEN_MASK;
    }
}

void DAC_SetBufferValue(DAC_Type *base, uint8_t index, uint16_t value);
static inline void DAC_DoSoftwareTriggerBuffer(DAC_Type *base)
{
    base->C0 |= DAC_C0_DACSWTRG_MASK;
}
static inline uint8_t DAC_GetBufferReadPointer(DAC_Type *base)
{
    return ((base->C2 & DAC_C2_DACBFRP_MASK) >> DAC_C2_DACBFRP_SHIFT);
}
void DAC_SetBufferReadPointer(DAC_Type *base, uint8_t index);

void DAC_EnableBufferInterrupts(DAC_Type *base, uint32_t mask);
void DAC_DisableBufferInterrupts(DAC_Type *base, uint32_t mask);
uint32_t DAC_GetBufferStatusFlags(DAC_Type *base);

void DAC_ClearBufferStatusFlags(DAC_Type *base, uint32_t mask);
#if defined(__cplusplus)
}
#endif
/*!
 * @}
 */
#endif /* _FSL_DAC_H_ */
