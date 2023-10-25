#include "../../../chickenFlap/dart/legacy/adc1023.h"

#ifdef HAL_ENABLE_LEGACY

uint16_t getADC1023(uint8_t channel) {
    // Convert from [0, ADC_MAX_VALUE] to [0, ADC1023_MAX_VALUE]
    return min(ADC1023_MAX_VALUE, getADC(channel) / (ADC_MAX_VALUE / ADC1023_MAX_VALUE));
}
#endif
