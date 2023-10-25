#include "../../../../chickenFlap/dart/hal_wrapper/dummy/spi.h"

#ifdef HAL_DUMMY_SPI

void hal_spi_init() {
	// Do nothing
}

void hal_spi_update() {
	// Do nothing
}

bool hal_spi_begin(int spi) {
	// Do nothing
	return false;
}

bool hal_spi_end(int spi) {
	// Do nothing
	return false;
}

bool hal_spi_write(int spi, uint8_t* data, size_t size) {
	// Do nothing
	return false;
}

bool hal_spi_read(int spi, uint8_t* data, size_t size) {
	// Do nothing
	return false;
}

bool hal_spi_transfer(int spi, uint8_t* tx, uint8_t* rx, size_t size) {
	// Do nothing
	return false;
}

bool hal_spi_set_speed(int spi, int frequency) {
	// Do nothing
	return false;
}
#endif
