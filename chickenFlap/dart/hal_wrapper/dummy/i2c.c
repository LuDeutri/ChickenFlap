#include "../../../../chickenFlap/dart/hal_wrapper/dummy/i2c.h"

#ifdef HAL_DUMMY_I2C

void hal_i2c_init() {
	// Do nothing
}

void hal_i2c_update() {
	// Do nothing
}

void hal_i2c_begin(int i2c) {
	// Do nothing
}

void hal_i2c_end(int i2c) {
	// Do nothing
}

bool hal_i2c_write(int i2c, uint8_t address, uint8_t* data, size_t size) {
	// Do nothing
	return false;
}

bool hal_i2c_read(int i2c, uint8_t address, uint8_t* data, size_t size) {
	// Do nothing
	return false;
}

bool hal_i2c_slave_ready(int i2c, uint8_t address) {
	// Do nothing
	return false;
}

#endif
