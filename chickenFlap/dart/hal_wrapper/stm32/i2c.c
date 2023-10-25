#include "../../../../chickenFlap/dart/hal_wrapper/stm32/i2c.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_I2C)
I2C_HandleTypeDef* i2cMap[I2C_COUNT] = I2C_MAP;

void hal_i2c_init() {
	// Do nothing
}

void hal_i2c_update() {
	// Do nothing
}

void hal_i2c_begin(int i2c) {
	DART_NOT_USED(i2c);
	// Do nothing
}

void hal_i2c_end(int i2c) {
	DART_NOT_USED(i2c);
	// Do nothing
}

bool hal_i2c_write(int i2c, uint8_t address, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(i2cMap[i2c], DART_ERROR_INTERNAL, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	return HAL_I2C_Master_Transmit(i2cMap[i2c], address << 1, data, size, 100) == HAL_OK;
}

bool hal_i2c_read(int i2c, uint8_t address, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(i2cMap[i2c], DART_ERROR_INTERNAL, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	return HAL_I2C_Master_Receive(i2cMap[i2c], address << 1, data, size, 100) == HAL_OK;
}

bool hal_i2c_slave_ready(int i2c, uint8_t address) {
	DART_NOT_NULL_RETURN(i2cMap[i2c], DART_ERROR_INTERNAL, false);
	return HAL_I2C_IsDeviceReady(i2cMap[i2c], address << 1, 2, 2) == HAL_OK;
}

#endif
