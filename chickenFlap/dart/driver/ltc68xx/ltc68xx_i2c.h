#ifndef DART_DRIVER_LTC68XX_LTC68XX_I2C_H_
#define DART_DRIVER_LTC68XX_LTC68XX_I2C_H_

#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The I2C bus is driven by the LTC68xx by half the SPI frequency.
 */

/**
 * This structure saves the data sent or read from a I2C slave.
 */
typedef struct {
    /**
     * When this is true, no data is sent or received from the I2C slave.
     */
    bool nop;
    /**
     * The 7-bit address of the I2C slave.
     */
    uint8_t addr;
    /**
     * The data to read or write to.
     */
    uint8_t* data;
    /**
     * The number of bytes to read or write.
     */
    size_t dataSize;
    /**
     * If the I2C command was successful.
     */
    bool successful;
} ltc68xx_i2c_data_t;


/**
 * Writes to the slave the register address to read.
 */
bool ltc68xx_i2c_begin_read_register8(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg);

/**
 * Reads the register for each slave and copies the 8 bit data to the given data buffer. The successful array contains if the read command was successful.
 */
bool ltc68xx_i2c_read_register8(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint8_t* data, size_t dataSize, bool* succesful, size_t successfulSize);

/**
 * Reads the register for each slave and copies the 16 bit data to the given data buffer. The successful array contains if the read command was successful.
 */
bool ltc68xx_i2c_read_register16(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint16_t* data, size_t dataSize, bool* succesful, size_t successfulSize);

/**
 * Reads data for each slave to the data array. dataSize is the size of the data array in bytes.
 */
bool ltc68xx_i2c_read(ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data, size_t dataSize);

/**
 * Writes a register for each slave. The successful array contains if the write command was successful.
 */
bool ltc68xx_i2c_write_register(ltc68xx_chain_t* chain, uint8_t addr, uint8_t* dataToWrite, size_t dataToWriteSize, bool* successful, size_t successfulSize);

/**
 * Writes a register for each slave, but with no data following. The successful array contains if the write command was successful.
 */
bool ltc68xx_i2c_write_register_no_data(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, bool* successful, size_t successfulSize);

/**
 * Writes a register for each slave with the given 8 bit data. The successful array contains if the write command was successful.
 */
bool ltc68xx_i2c_write_register8(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint8_t data, bool* successful, size_t successfulSize);

/**
 * Writes a register for each slave with the given 16 bit data. The successful array contains if the write command was successful.
 */
bool ltc68xx_i2c_write_register16(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint16_t data, bool* successful, size_t successfulSize);

/**
 * Writes each data item to each slave of the chain. dataSize is the size of the data array in bytes.
 */
bool ltc68xx_i2c_write(ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data, size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif
