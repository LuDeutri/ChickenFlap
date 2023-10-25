#ifndef DART_SPI_H_
#define DART_SPI_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the SPI module.
 */
void dartSPI_init();

/**
 * Updates the SPI module.
 */
void dartSPI_update();

/**
 * Begins a SPI transaction.
 */
bool spi_begin(int spi);

/**
 * Ends a SPI transaction.
 */
bool spi_end(int spi);

/**
 * Writes and reads data on the SPI bus. The data read is returned.
 */
uint8_t spi_transfer8(int spi, uint8_t data);
/**
 * Writes and reads data on the SPI bus. The data read is returned.
 */
uint16_t spi_transfer16(int spi, uint16_t data);
/**
 * Writes and reads data on the SPI bus.
 */
bool spi_transfer(int spi, uint8_t* tx, uint8_t* rx, size_t size);

/**
 * Write on the SPI bus.
 */
bool spi_write(int spi, uint8_t* data, size_t size);

/**
 * Reads data on the SPI bus.
 */
bool spi_read(int spi, uint8_t* data, size_t size);

/**
 * Sets the SPI bus speed.
 */
bool spi_set_speed(int spi, int frequency);

/**
 * Initializes the HAL behind the SPI module. Implemented by the HAL wrapper.
 */
void hal_spi_init();
/**
 * Updates the HAL behind the SPI module. Implemented by the HAL wrapper.
 */
void hal_spi_update();
/**
 * Begins a SPI transaction. Implemented by the HAL wrapper.
 */
bool hal_spi_begin(int spi);
/**
 * Ends a SPI transaction. Implemented by the HAL wrapper.
 */
bool hal_spi_end(int spi);
/**
 * Writes data on the SPI bus. Implemented by the HAL wrapper.
 */
bool hal_spi_write(int spi, uint8_t* data, size_t size);
/**
 * Reads data on the SPI bus. Implemented by the HAL wrapper.
 */
bool hal_spi_read(int spi, uint8_t* data, size_t size);
/**
 * Reads and writes data on the SPI bus. Implemented by the HAL wrapper.
 */
bool hal_spi_transfer(int spi, uint8_t* tx, uint8_t* rx, size_t size);
/**
 * Sets the SPI bus speed. Implemented by the HAL wrapper.
 */
bool hal_spi_set_speed(int spi, int frequency);

#ifdef __cplusplus
}
#endif

#endif
