#ifndef DART_HAL_WRAPPER_ATMEGA32M1_I2C_H_
#define DART_HAL_WRAPPER_ATMEGA32M1_I2C_H_

#include "../../../../chickenFlap/dart/i2c.h"

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_DATA_HI() do { I2C_DATA_DDR &= ~ (1 << I2C_DATA_PIN_NUMBER); I2C_DATA_PORT |= (1 << I2C_DATA_PIN_NUMBER); } while (0);
#define I2C_DATA_LO() do { I2C_DATA_DDR |= (1 << I2C_DATA_PIN_NUMBER); I2C_DATA_PORT &= ~ (1 << I2C_DATA_PIN_NUMBER); } while (0);

#define I2C_CLOCK_HI() do { I2C_CLOCK_DDR &= ~ (1 << I2C_CLOCK_PIN_NUMBER); I2C_CLOCK_PORT |= (1 << I2C_CLOCK_PIN_NUMBER); } while(0);
#define I2C_CLOCK_LO() do { I2C_CLOCK_DDR |= (1 << I2C_CLOCK_PIN_NUMBER); I2C_CLOCK_PORT &= ~ (1 << I2C_CLOCK_PIN_NUMBER); } while(0);

void _hal_i2c_writeBit(unsigned char c);
unsigned char _hal_i2c_readBit();

unsigned char _hal_i2c_write(unsigned char c);
unsigned char _hal_i2c_read(unsigned char ack);

#ifdef __cplusplus
}
#endif


#endif
