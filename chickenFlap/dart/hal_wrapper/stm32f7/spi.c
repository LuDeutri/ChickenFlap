#include "../../../../chickenFlap/dart/hal_wrapper/stm32f7/spi.h"

#if defined(USE_STM32F7) && defined(HAL_ENABLE_SPI)
extern SPI_HandleTypeDef* spiMap[SPI_COUNT];

bool hal_spi_set_speed(int spi, int frequency) {
	SPI_HandleTypeDef* handle = spiMap[spi];
	DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);

	int clk = HAL_RCC_GetPCLK2Freq();
	if (frequency >= clk / 2)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	else if (frequency >= clk / 4)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	else if (frequency >= clk / 8)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	else if (frequency >= clk / 16)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	else if (frequency >= clk / 32)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	else if (frequency >= clk / 64)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	else if (frequency >= clk / 128)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	else
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	return HAL_SPI_Init(handle) == HAL_OK;
}
#endif
