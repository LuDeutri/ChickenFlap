#ifndef DART_DRIVER_INA260_H_
#define DART_DRIVER_INA260_H_

#include "../../../chickenFlap/dart/i2c.h"
#include "../../../chickenFlap/dart/util/filter.h"
#include "../../../chickenFlap/dart/util/motorola.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C Base Address
#define INA260_ADDR_BASE 64

// I2C Slave Addresses
#define INA260_ADDR_0 (INA260_ADDR_BASE + 0)
#define INA260_ADDR_1 (INA260_ADDR_BASE + 1)
#define INA260_ADDR_2 (INA260_ADDR_BASE + 2)
#define INA260_ADDR_3 (INA260_ADDR_BASE + 3)
#define INA260_ADDR_4 (INA260_ADDR_BASE + 4)
#define INA260_ADDR_5 (INA260_ADDR_BASE + 5)
#define INA260_ADDR_6 (INA260_ADDR_BASE + 6)
#define INA260_ADDR_7 (INA260_ADDR_BASE + 7)
#define INA260_ADDR_8 (INA260_ADDR_BASE + 8)
#define INA260_ADDR_9 (INA260_ADDR_BASE + 9)
#define INA260_ADDR_10 (INA260_ADDR_BASE + 10)
#define INA260_ADDR_11 (INA260_ADDR_BASE + 11)
#define INA260_ADDR_12 (INA260_ADDR_BASE + 12)
#define INA260_ADDR_13 (INA260_ADDR_BASE + 13)
#define INA260_ADDR_14 (INA260_ADDR_BASE + 14)
#define INA260_ADDR_15 (INA260_ADDR_BASE + 15)

// Configuration Register: All-register reset,shunt voltage and bus voltage ADC conversion times and averaging, operating mode.
#define INA260_REG_CONFIG 0x00
// Current Register: Contains the value of the current flowing through the shunt resistor.
#define INA260_REG_CURRENT 0x01
// Bus Voltage Register: Bus voltage measurement data.
#define INA260_REG_BUS_VOLTAGE 0x02
// Power Register: Contains the value of the calculated power being delivered to the load.
#define INA260_REG_POWER 0x03
// Mask/Enable Register: Alert configuration and Conversion Ready flag
#define INA260_REG_MASK_ENABLE 0x04
// Alert Limit Register: Contains the limit value to compare to the selected Alert function.
#define INA260_REG_ALERT_LIMIT 0x05
// Manufacturer ID Register: Contains unique manufacturer identification number.
#define INA260_REG_MANUFACTURER_ID 0xFE
// Die ID Register: Contains unique die identification number.
#define INA260_REG_DIE_ID 0xFF

// Resets all registers to default values; this bit self-clears.
#define INA260_CONFIG_RESET (1 << 15)

// Averaging Mode: Determines the number of samples that are collected and averaged.
#define INA260_CONFIG_AVG_1 ((0u << 11) | (0u << 10) | (0u << 9))
#define INA260_CONFIG_AVG_4 ((0u << 11) | (0u << 10) | (1u << 9))
#define INA260_CONFIG_AVG_16 ((0u << 11) | (1u << 10) | (0u << 9))
#define INA260_CONFIG_AVG_64 ((0u << 11) | (1u << 10) | (1u << 9))
#define INA260_CONFIG_AVG_128 ((1u << 11) | (0u << 10) | (0u << 9))
#define INA260_CONFIG_AVG_256 ((1u << 11) | (0u << 10) | (1u << 9))
#define INA260_CONFIG_AVG_512 ((1u << 11) | (1u << 10) | (0u << 9))
#define INA260_CONFIG_AVG_1024 ((1u << 11) | (1u << 10) | (1u << 9))
#define INA260_CLEAR_AVG ((1u << 11) | (1u << 10) | (1u << 9))

// Bus Voltage Conversion Time: Sets the conversion time for the bus voltage measurement.
#define INA260_CONFIG_VBUSCT_140uS ((0u << 8) | (0u << 7) | (0u << 6))
#define INA260_CONFIG_VBUSCT_204uS ((0u << 8) | (0u << 7) | (1u << 6))
#define INA260_CONFIG_VBUSCT_332uS ((0u << 8) | (1u << 7) | (0u << 6))
#define INA260_CONFIG_VBUSCT_588uS ((0u << 8) | (1u << 7) | (1u << 6))
#define INA260_CONFIG_VBUSCT_1_1ms ((1u << 8) | (0u << 7) | (0u << 6))
#define INA260_CONFIG_VBUSCT_2_116ms ((1u << 8) | (0u << 7) | (1u << 6))
#define INA260_CONFIG_VBUSCT_4_156ms ((1u << 8) | (1u << 7) | (0u << 6))
#define INA260_CONFIG_VBUSCT_8_224ms ((1u << 8) | (1u << 7) | (1u << 6))
#define INA260_CLEAR_VBUST_CT ((1u << 8) | (1u << 7) | (1u << 6))

// Shunt Current Conversion Time
#define INA260_CONFIG_ISHCT_140uS ((0u << 5) | (0u << 4) | (0u << 3))
#define INA260_CONFIG_ISHCT_204uS ((0u << 5) | (0u << 4) | (1u << 3))
#define INA260_CONFIG_ISHCT_332uS ((0u << 5) | (1u << 4) | (0u << 3))
#define INA260_CONFIG_ISHCT_588uS ((0u << 5) | (1u << 4) | (1u << 3))
#define INA260_CONFIG_ISHCT_1_1ms ((1u << 5) | (0u << 4) | (0u << 3))
#define INA260_CONFIG_ISHCT_2_116ms ((1u << 5) | (0u << 4) | (1u << 3))
#define INA260_CONFIG_ISHCT_4_156ms ((1u << 5) | (1u << 4) | (0u << 3))
#define INA260_CONFIG_ISHCT_8_224ms ((1u << 5) | (1u << 4) | (1u << 3))
#define INA260_CLEAR_ISHCT ((1u << 5) | (1u << 4) | (1u << 3))

// Operating Mode: Selects continuous, triggered, or power-down mode of operation.
#define INA260_CONFIG_MODE_POWER_DOWN ((0u << 2) | (0u << 1) | (0u << 0))
#define INA260_CONFIG_MODE_CURRENT_TRIGGER ((0u << 2) | (0u << 1) | (1u << 0))
#define INA260_CONFIG_MODE_VOLTAGE_TRIGGER ((0u << 2) | (1u << 1) | (0u << 0))
#define INA260_CONFIG_MODE_CURRENT_VOLTAGE_TRIGGER ((0u << 2) | (1u << 1) | (1u << 0))

#define INA260_CONFIG_MODE_CURRENT_CONT ((1u << 2) | (0u << 1) | (1u << 0))
#define INA260_CONFIG_MODE_VOLTAGE_CONT ((1u << 2) | (1u << 1) | (0u << 0))
#define INA260_CONFIG_MODE_CURRENT_VOLTAGE_CONT ((1u << 2) | (1u << 1) | (1u << 0))
#define INA260_CLEAR_MODE ((1u << 2) | (1u << 1) | (1u << 0))

#define INA260_CLEAR_ALERT ((1u << 15) | (1u << 14) | (1u << 13) | (1u << 12) | (1u << 11) | (1u << 10))

// Current scale
#define INA260_CURRENT_SCALE (0.00125f) // A

// Bus Voltage scale
#define INA260_VOLTAGE_SCALE (0.00125f) // V

// Power scale
#define INA260_POWER_SCALE (0.01f) // W

typedef struct {
	int bus;
	uint8_t device;

	uint16_t configRegister;
	uint16_t maskEnableRegister;

	bool enableVoltageFilter;
	float lastVoltage;

	bool enableCurrentFilter;
	float lastCurrent;
} ina260_device_t;

bool ina260_init(ina260_device_t* device);
bool ina260_reset(ina260_device_t* device);
bool ina260_power_down(ina260_device_t* device);

bool ina260_enable_voltage_filter(ina260_device_t* device, bool enable);
bool ina260_enable_current_filter(ina260_device_t* device, bool enable);

bool ina260_read_manufacturer_id(ina260_device_t* device, uint16_t* value);
bool ina260_read_device_id(ina260_device_t* device, uint16_t* value);
bool ina260_read_die_revision(ina260_device_t* device, uint16_t* value);

bool ina260_read_current(ina260_device_t* device, float* value);
bool ina260_read_bus_voltage(ina260_device_t* device, float* value);
bool ina260_read_power(ina260_device_t* device, float* value);

bool ina260_write_mode(ina260_device_t* device, uint16_t mode);
bool ina260_write_averaging(ina260_device_t* device, uint16_t averaging);
bool ina260_write_bus_voltage_conversion_time(ina260_device_t* device, uint16_t time);
bool ina260_write_current_conversion_time(ina260_device_t* device, uint16_t time);

bool ina260_set_alert_current_over(ina260_device_t* device, float current);
bool ina260_set_alert_current_under(ina260_device_t* device, float current);
bool ina260_set_alert_voltage_over(ina260_device_t* device, float voltage);
bool ina260_set_alert_voltage_under(ina260_device_t* device, float voltage);
bool ina260_set_alert_power_over(ina260_device_t* device, float power);
bool ina260_set_alert_conversion_ready(ina260_device_t* device);

#ifdef __cplusplus
}
#endif

#endif
