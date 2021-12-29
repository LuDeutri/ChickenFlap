#include "../../chickenFlap/dart/spi.h"

void dartSPI_init() {
	hal_spi_init();
}

void dartSPI_update() {
	hal_spi_update();
}

bool spi_begin(int spi) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, false);
	return hal_spi_begin(spi);
}

bool spi_end(int spi) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, false);
	return hal_spi_end(spi);
}

uint8_t spi_transfer8(int spi, uint8_t data) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, 0);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, 0);

	uint8_t rx = 0;
	spi_transfer(spi, &data, &rx, sizeof(data));
	return rx;
}

uint16_t spi_transfer16(int spi, uint16_t data) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, 0);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, 0);

	uint16_t rx = 0;
	spi_transfer(spi, (uint8_t*)&data, (uint8_t*)&rx, sizeof(data));
	return rx;
}

bool spi_transfer(int spi, uint8_t* tx, uint8_t* rx, size_t size) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(tx, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(rx, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size > 0, DART_ERROR_INVALID_VALUE, false);

	return hal_spi_transfer(spi, tx, rx, size);
}

bool spi_write(int spi, uint8_t* data, size_t size) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size > 0, DART_ERROR_INVALID_VALUE, false);

	return hal_spi_write(spi, data, size);
}


bool spi_read(int spi, uint8_t* data, size_t size) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size > 0, DART_ERROR_INVALID_VALUE, false);

	return hal_spi_read(spi, data, size);
}

bool spi_set_speed(int spi, int frequency) {
	DART_ASSERT_RETURN(spi >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(spi < SPI_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(frequency > 0, DART_ERROR_INVALID_VALUE, false);
	return hal_spi_set_speed(spi, frequency);
}
