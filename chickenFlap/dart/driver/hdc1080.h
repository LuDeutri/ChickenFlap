#ifndef DART_DRIVER_HDC1080_H_
#define DART_DRIVER_HDC1080_H_

#include "../../../chickenFlap/dart/i2c.h"
#include "../../../chickenFlap/dart/util/endianness.h"

#define HDC1080_I2C_ADDRESS 64

#define HDC1080_REG_TEMPERATURE 0
#define HDC1080_REG_HUMIDITY 1
#define HDC1080_REG_CONFIGURATION 2
#define HDC1080_REG_SERIAL_ID_1 0xFB
#define HDC1080_REG_SERIAL_ID_2 0xFC
#define HDC1080_REG_SERIAL_ID_3 0xFD
#define HDC1080_REG_MANUFACTURER_ID 0xFE
#define HDC1080_REG_DEVICE_ID 0xFF

#define HDC1080_MANUFACTURER_ID 0x5449
#define HDC1080_DEVICE_ID 0x1050

#define HDC1080_CONFIG_SW_RESET (1 << 15)

#define HDC1080_CONFIG_HEATER_ENABLED (1 << 13)
#define HDC1080_CONFIG_HEATER_DISABLED (0 << 13)

#define HDC1080_CONFIG_MEASURE_BOTH (1 << 12)

#define HDC1080_CONFIG_TEMP_14BIT (0 << 10)
#define HDC1080_CONFIG_TEMP_11BIT (1 << 10)

#define HDC1080_CONFIG_HUMIDITY_14BIT ((0 << 9) | (0 << 8))
#define HDC1080_CONFIG_HUMIDITY_11BIT ((0 << 9) | (1 << 8))
#define HDC1080_CONFIG_HUMIDITY_8BIT ((1 << 9) | (0 << 8))


#ifdef __cplusplus
extern "C" {
#endif

float hdc1080_convert_temperature(uint16_t value);
float hdc1080_convert_humidity(uint16_t value);

#ifdef __cplusplus
}
#endif

#endif
