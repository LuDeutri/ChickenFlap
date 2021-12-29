#include "../../../../chickenFlap/dart/hal_wrapper/stm32/uart.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_SPI)
SPI_HandleTypeDef* spiMap[SPI_COUNT] = SPI_MAP;

void hal_spi_init() {
	// Everything done in main.c
}

void hal_spi_update() {
	// Do nothing
}

bool hal_spi_begin(int spi) {
	DART_NOT_USED(spi);
	// Do nothing
	return true;
}

bool hal_spi_end(int spi) {
	DART_NOT_USED(spi);
	// Do nothing
	return true;
}

bool hal_spi_write(int spi, uint8_t* data, size_t size) {
	SPI_HandleTypeDef* handle = spiMap[spi];
	DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);

	return HAL_SPI_Transmit(handle, data, size, 100) == HAL_OK;
}

bool hal_spi_read(int spi, uint8_t* data, size_t size) {
	SPI_HandleTypeDef* handle = spiMap[spi];
	DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);

	return HAL_SPI_Receive(handle, data, size, 100) == HAL_OK;
}

bool hal_spi_transfer(int spi, uint8_t* tx, uint8_t* rx, size_t size) {
	SPI_HandleTypeDef* handle = spiMap[spi];
	DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);

	return HAL_SPI_TransmitReceive(handle, tx, rx, size, 100) == HAL_OK;
}
#endif
