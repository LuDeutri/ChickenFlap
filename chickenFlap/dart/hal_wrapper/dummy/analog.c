#include "../../../../chickenFlap/dart/hal_wrapper/dummy/analog.h"

#ifdef HAL_DUMMY_ADC

void hal_adc_init() {
	// Do nothing
}

void hal_adc_update() {
	// Do nothing
}

uint16_t hal_adc_get(int channel) {
	return ADC_MAX_VALUE / 2;
}

#endif
