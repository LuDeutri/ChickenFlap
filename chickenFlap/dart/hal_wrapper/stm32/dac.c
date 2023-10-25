#include "../../../../chickenFlap/dart/hal_wrapper/stm32/dac.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_DAC)
DAC_HandleTypeDef* dacPeriferalMap[DAC_CHANNEL_COUNT] = DAC_MAP_PERIPHERAL;
uint8_t dacCannelMapping[DAC_CHANNEL_COUNT] = DAC_MAP_CHANNEL;

void dartDAC_init() {
	for(uint8_t i = 0; i < DAC_CHANNEL_COUNT; i++)
		HAL_DAC_Start(dacPeriferalMap[i], dacCannelMapping[i]);
}

void dartDAC_update() {
	// Nothing to do
}

bool dacWrite(uint8_t channel, uint32_t data){
	DART_ASSERT_RETURN(channel < DAC_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(dacPeriferalMap[channel], DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(data <= DAC_MAX_VALUE, DART_ERROR_INVALID_VALUE, false);

	return HAL_DAC_SetValue(dacPeriferalMap[channel], dacCannelMapping[channel], DAC_ALIGN_12B_R, data) == HAL_OK;
}
#endif
