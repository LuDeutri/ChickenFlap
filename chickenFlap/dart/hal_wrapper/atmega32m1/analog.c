#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/analog.h"

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_ADC)
uint16_t adcValues[ADC_CHANNEL_COUNT];
int adcChannelMapping[ADC_CHANNEL_COUNT] = ADC_MAP_CHANNELS;

void hal_adc_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
	memset(adcValues, 0, sizeof(adcValues));
}

void hal_adc_update() {
	for (int i = 0; i < ADC_CHANNEL_COUNT; i++) {
		// Select ADC channel
		ADMUX = (1 << REFS0) | adcChannelMapping[i];
		// Start sample and wait for result
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		adcValues[i] = ADC;
	}
}

uint16_t hal_adc_get(int channel) {
	return adcValues[channel];
}
#endif
