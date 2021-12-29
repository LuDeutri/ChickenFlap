#include "../../../../chickenFlap/dart/hal_wrapper/tms570/analog.h"

#if defined(USE_TMS570) && defined(HAL_ENABLE_ADC)
uint16_t adcValues[ADC_CHANNEL_COUNT];

/**
 * Temporary buffer to save the FIFO data that is read by adcGetData().
 */
adcData_t _hal_adcData[HAL_CONFIG_ADC1_GROUP1_FIFO_SIZE];

void hal_adc_init() {
    adcInit();

    // Start first conversion
    #ifdef ADC_ENABLE_GROUP1
        adcStartConversion(adcREG1, adcGROUP1);
    #endif
    #ifdef ADC_ENABLE_GROUP2
        adcStartConversion(adcREG1, adcGROUP2);
    #endif
}

void hal_adc_update() {
    #ifdef ADC_ENABLE_GROUP1
        hal_adc_update_group(adcGROUP1);
    #endif
    #ifdef ADC_ENABLE_GROUP2
        hal_adc_update_group(adcGROUP2);
    #endif
}

void hal_adc_update_group(uint8_t group) {
    // Wait for conversion
    util_timer_t timeout = createTimerNoOffset(ADC_TIMEOUT);
    while(adcIsConversionComplete(adcREG1, group) == ADC_CONVERSION_IS_NOT_FINISHED) {
       if (hasTimerElapsed(&timeout)) {
           // Restart conversion
           adcStopConversion(adcREG1, group);
           adcStartConversion(adcREG1, group);
           DART_ERROR(DART_ERROR_TIMEOUT);
       }
    }

    // Read out data
    uint32_t count = adcGetData(adcREG1, group, _hal_adcData);

    // Start next ADC conversion
    adcStartConversion(adcREG1, group);

    // Check for buffer overflow
    DART_ASSERT(count <= HAL_CONFIG_ADC1_GROUP1_FIFO_SIZE, DART_ERROR_BUFFER_OVERFLOW);

    // Copy data from FIFO to our buffer
    for (size_t i = 0; i < count; i++) {
       adcData_t data = _hal_adcData[i];

       // Copy value
       if (data.id < ADC_CHANNEL_COUNT) // Check for overflow
           adcValues[data.id] = data.value;
       else
           DART_ERROR(DART_ERROR_HAL);
    }
}

uint16_t hal_adc_get(int channel) {
    return adcValues[channel];
}
#endif
