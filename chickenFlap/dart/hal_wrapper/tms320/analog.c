#include "../../../../chickenFlap/dart/hal_wrapper/tms320/analog.h"

#if defined(USE_TMS320) && defined(HAL_ENABLE_ADC)
uint16_t adcValues[ADC_CHANNEL_COUNT];

void hal_adc_init() {
    DART_NOT_IMPLEMENTED();
}

void hal_adc_update() {
    DART_NOT_IMPLEMENTED();
}

uint16_t hal_adc_get(int channel) {
    return adcValues[channel];
}
#endif
