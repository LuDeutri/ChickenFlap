#ifndef DART_DRIVER_MAX31725_H_
#define DART_DRIVER_MAX31725_H_

#include "../../../chickenFlap/dart/i2c.h"
#include "../../../chickenFlap/dart/util/endianness.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C Slave Addresses (compatible with HAL addresses 0-127)

// Slave: A2, A1, A0
// Slave: GND, GND, GND
#define MAX31725_ADDR_0 (0x90 >> 1)
// Slave: GND, GND, Vdd
#define MAX31725_ADDR_1 (0x92 >> 1)
// Slave: GND, GND, SCL
#define MAX31725_ADDR_2 (0x82 >> 1)
// Slave: GND, GND, SDA
#define MAX31725_ADDR_3 (0x80 >> 1)
// Slave: GND, VDD, GND
#define MAX31725_ADDR_4 (0x94 >> 1)
// Slave: GND, VDD, VDD
#define MAX31725_ADDR_5 (0x96 >> 1)
// Slave: GND, VDD, SCL
#define MAX31725_ADDR_6 (0x86 >> 1)
// Slave: GND, VDD, SDA
#define MAX31725_ADDR_7 (0x84 >> 1)
// Slave: GND, SCL, GND
#define MAX31725_ADDR_8 (0xB4 >> 1)
// Slave: GND, SCL, VDD
#define MAX31725_ADDR_9 (0xB6 >> 1)
// Slave: GND, SCL, SCL
#define MAX31725_ADDR_10 (0xA6 >> 1)
// Slave: GND, SCL, SDA
#define MAX31725_ADDR_11 (0xA4 >> 1)
// Slave: GND, SDA, GND
#define MAX31725_ADDR_12 (0xB0 >> 1)
// Slave: GND, SDA, VDD
#define MAX31725_ADDR_13 (0xB2 >> 1)
// Slave: GND, SDA, SCL
#define MAX31725_ADDR_14 (0xA2 >> 1)
// Slave: GND, SDA, SDA
#define MAX31725_ADDR_15 (0xA0 >> 1)
// Slave: VDD, GND, GND
#define MAX31725_ADDR_16 (0x98 >> 1)
// Slave: VDD, GND, VDD
#define MAX31725_ADDR_17 (0x9A >> 1)
// Slave: VDD, GND, SCL
#define MAX31725_ADDR_18 (0x8A >> 1)
// Slave: VDD, GND, SDA
#define MAX31725_ADDR_19 (0x88 >> 1)
// Slave: VDD, VDD, GND
#define MAX31725_ADDR_20 (0x9C >> 1)
// Slave: VDD, VDD, VDD
#define MAX31725_ADDR_21 (0x9E >> 1)
// Slave: VDD, VDD, SCL
#define MAX31725_ADDR_22 (0x8E >> 1)
// Slave: VDD, VDD, SDA
#define MAX31725_ADDR_23 (0x8C >> 1)
// Slave: VDD, SCL, GND
#define MAX31725_ADDR_24 (0xBC >> 1)
// Slave: VDD, SCL, VDD
#define MAX31725_ADDR_25 (0xBE >> 1)
// Slave: VDD, SCL, SCL
#define MAX31725_ADDR_26 (0xAE >> 1)
// Slave: VDD, SCL, SDA
#define MAX31725_ADDR_27 (0xAC >> 1)
// Slave: VDD, SDA, GND
#define MAX31725_ADDR_28 (0xB8 >> 1)
// Slave: VDD, SDA, VDD
#define MAX31725_ADDR_29 (0xBA >> 1)
// Slave: VDD, SDA, SCL
#define MAX31725_ADDR_30 (0xAA >> 1)
// Slave: VDD, SDA, SDA
#define MAX31725_ADDR_31 (0xA8 >> 1)

#define MAX31725_REG_TEMPERATURE 0x0
#define MAX31725_REG_CONFIG 0x1
#define MAX31725_REG_T_HYST 0x2
#define MAX31725_REG_T_OS 0x3

#define MAX31725_T_HYST_DEFAULT 0x4B00
#define MAX31725_T_OS_DEFAULT 0x5000

// Config values
#define MAX31725_CONFIG_ONE_SHOT (1 << 7)
#define MAX31725_CONFIG_DATA_EXTENDED (1 << 5)
#define MAX31725_CONFIG_SHUTDOWN (1 << 0)

// Temperature Scale
#define MAX31725_TEMPERATURE_SCALE 0.00390625f // �C
#define MAX31725_TEMPERATURE_EXTENDED_OFFSET 64.0f // �C

typedef struct {
	int bus;
	uint8_t address;

	uint8_t configRegister;
} max31725_device_t;

bool max31725_init(max31725_device_t* device);
bool max31725_wakeup(max31725_device_t* device);
bool max31725_shutdown(max31725_device_t* device);
bool max31725_oneshot_measure(max31725_device_t* device);

bool max31725_is_extended_data(max31725_device_t* device);
bool max31725_set_extended_data(max31725_device_t* device, bool enable);

bool max31725_read_temperature(max31725_device_t* device, float* value);

#ifdef __cplusplus
}
#endif

#endif
