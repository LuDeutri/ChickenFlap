#ifndef DART_DRIVER_LTC68XX_LTC68XX_DATA_H_
#define DART_DRIVER_LTC68XX_LTC68XX_DATA_H_

#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_base.h"
#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns true when the LTC device as any internal error.
 */
bool ltc68xx_has_error(const ltc68xx_t device);

/**
 * Returns the SID of the device.
 */
uint64_t ltc68xx_get_sid(const ltc68xx_t device);

/**
 * Returns the measured die temperature of the LTC device in �C.
 */
float ltc68xx_get_die_temperature(const ltc68xx_t device);
/**
 * Returns the measured reference voltage of the LTC device in V.
 */
float ltc68xx_get_ref_voltage(const ltc68xx_t device);
/**
 * Returns the internal analog supply voltage of the LTC device in V.
 */
float ltc68xx_get_analog_supply_voltage(const ltc68xx_t device);
/**
 * Returns the internal digital supply voltage of the LTC device in V.
 */
float ltc68xx_get_digital_supply_voltage(const ltc68xx_t device);

/**
 * Returns the raw measured cell voltage (the ADC value) at the given index (index 0 is the first cell). The value 0 is returned for indices which are over the limit for the LTC device.
 */
uint16_t ltc68xx_get_raw_cell_voltage(const ltc68xx_t device, uint8_t index);

/**
 * Returns the measured cell voltage in V at the given index (index 0 is the first cell). The value 0 is returned for indices which are over the limit for the LTC device.
 */
float ltc68xx_get_cell_voltage(const ltc68xx_t device, uint8_t index);

/**
 * Sets the voltage threshold for the built-in under voltage detection.
 */
bool ltc68xx_set_undervoltage(ltc68xx_chain_t* chain, float voltage);

/**
 * Sets the voltage threshold for the built-in over voltage detection.
 */
bool ltc68xx_set_overvoltage(ltc68xx_chain_t* chain, float voltage);

/**
 * Enables (true) or disables (false) the discharge for the given cell (index 0 is the first cell) for the given LTC device at the given index.
 */
bool ltc68xx_set_discharge(ltc68xx_chain_t* chain, int deviceIndex, uint8_t cell, bool state, bool writeConfigToChain);

/**
 * Sets the given GPIO pull-down (index 0 is the first pin) for the given LTC device at the given index. When state is true the pull-down is enabled.
 */
bool ltc68xx_set_gpio_pulldown(ltc68xx_chain_t* chain, int deviceIndex, uint8_t gpio, bool state, bool writeConfigToChain);

/**
 * Returns the logic level of the given GPIO pin on the given LTC device in the given chain. Returns true when the pin is HIGH and false otherwise.
 */
bool ltc68xx_get_gpio(const ltc68xx_t device, uint8_t gpio);

/**
 * Returns the measured GPIO voltage in V at the given index (index 0 is the first GPIO). The value 0 is returned for indices which are over the limit for the LTC device.
 */
float ltc68xx_get_gpio_voltage(const ltc68xx_t device, uint8_t gpio);


#ifdef __cplusplus
}
#endif

#endif
