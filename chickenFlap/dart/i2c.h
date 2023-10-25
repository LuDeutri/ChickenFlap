#ifndef DART_I2C_H_
#define DART_I2C_H_

#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MAX_SLAVE_COUNT 128

/**
 * Initializes the I2C module.
 */
void dartI2C_init();

/**
 * Updates the I2C module.
 */
void dartI2C_update();

/**
 * Begins a I2C transaction.
 */
void i2c_begin(int i2c);

/**
 * Ends a I2C transaction.
 */
void i2c_end(int i2c);

/**
 * Writes data on the I2C bus.
 */
bool i2c_write(int i2c, uint8_t address, uint8_t* data, size_t size);
/**
 * Reads data on the I2C bus.
 */
bool i2c_read(int i2c, uint8_t address, uint8_t* data, size_t size);

/**
 * Writes data on the I2C bus.
 */
bool i2c_write_register(int i2c, uint8_t address, uint8_t regAddr, uint8_t* data, size_t size);
/**
 * Reads data on the I2C bus.
 */
bool i2c_read_register(int i2c, uint8_t address, uint8_t regAddr, uint8_t* data, size_t size);

/**
 * Checks if the slave is ready. Implemented by the HAL wrapper.
 */
bool i2c_slave_ready(int i2c, uint8_t address);

/**
 * Initializes the HAL behind the I2C module. Implemented by the HAL wrapper.
 */
void hal_i2c_init();
/**
 * Updates the HAL behind the I2C module. Implemented by the HAL wrapper.
 */
void hal_i2c_update();
/**
 * Begins a I2C transaction. Implemented by the HAL wrapper.
 */
void hal_i2c_begin(int i2c);
/**
 * Ends a I2C transaction. Implemented by the HAL wrapper.
 */
void hal_i2c_end(int i2c);

/**
 * Writes data on the I2C bus. Implemented by the HAL wrapper.
 */
bool hal_i2c_write(int i2c, uint8_t address, uint8_t* data, size_t size);
/**
 * Reads data on the I2C bus. Implemented by the HAL wrapper.
 */
bool hal_i2c_read(int i2c, uint8_t address, uint8_t* data, size_t size);

/**
 * Checks if the slave is ready. Implemented by the HAL wrapper.
 */
bool hal_i2c_slave_ready(int i2c, uint8_t address);

#ifdef __cplusplus
}
#endif

#endif
