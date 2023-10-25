#include "../../../../chickenFlap/dart/hal_wrapper/stm32/analog.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_ADC)

#ifndef ADC_CHANNEL_COUNT
#error "ADC_CHANNEL_COUNT is not defined"
#endif

#ifndef ADC_MAP
#error "ADC_MAP is not defined"
#endif

#ifndef ADC_MAP_CHANNELS
#error "ADC_MAP_CHANNELS is not defined"
#endif

uint16_t adcValues[ADC_CHANNEL_COUNT];

ADC_HandleTypeDef* adcMap[ADC_CHANNEL_COUNT] = ADC_MAP;
int adcChannelMapping[ADC_CHANNEL_COUNT] = ADC_MAP_CHANNELS;

void hal_adc_init() {
	// HAL done in main.c
	memset(adcValues, 0, sizeof(adcValues));
}

void hal_adc_update() {
	static int currentIndex = 0;

	// Update two channels each update
	for (int i = 0; i < 2; i++) {
		ADC_HandleTypeDef* adc = adcMap[currentIndex];
		DART_NOT_NULL(adc, DART_ERROR_INTERNAL);

		ADC_ChannelConfTypeDef channel;
		channel.Channel = adcChannelMapping[currentIndex];
		channel.Rank = ADC_REGULAR_RANK_1;
		channel.SamplingTime = ANALOG_SAMPLE_TIME;

		// Configure channel, start conversion, wait for it
		DART_ASSERT_HAL_OK(HAL_ADC_ConfigChannel(adc, &channel));
		DART_ASSERT_HAL_OK(HAL_ADC_Start(adc));
		DART_ASSERT_HAL_OK(HAL_ADC_PollForConversion(adc, 50));

		// Get value and stop conversion
		adcValues[currentIndex] = HAL_ADC_GetValue(adc);
		HAL_ADC_Stop(adc);

		currentIndex++;
		if (currentIndex >= ADC_CHANNEL_COUNT)
			currentIndex = 0;
	}
}

uint16_t hal_adc_get(int channel) {
	return adcValues[channel];
}
#endif
