#ifndef DART_DRIVER_PCA9538_H_
#define DART_DRIVER_PCA9538_H_

#include "../../../chickenFlap/dart/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PCA9538_ADDR_BASE (0b1110000)

// I2C Slave Addresses
#define PCA9538_ADDR_0 (PCA9538_ADDR_BASE + 0)
#define PCA9538_ADDR_1 (PCA9538_ADDR_BASE + 1)
#define PCA9538_ADDR_2 (PCA9538_ADDR_BASE + 2)
#define PCA9538_ADDR_3 (PCA9538_ADDR_BASE + 3)

// Registers
#define PCA9538_REG_INPUT 0x0
#define PCA9538_REG_OUTPUT 0x1
#define PCA9538_REG_POLARITY 0x2
#define PCA9538_REG_CONFIG 0x3

typedef struct {
	int bus;
	uint8_t device;

	uint8_t inputRegister;
	uint8_t outputRegister;
	uint8_t polarityRegister;
	uint8_t configRegister;
} pca9538_device_t;

typedef enum {
	pca9538_mode_output,
	pca9538_mode_input,
} pca9538_mode_t;

bool pca9538_init(pca9538_device_t* device);
bool pca9538_read_input(pca9538_device_t* device);
bool pca9538_write_output(pca9538_device_t* device);

bool pca9538_write_mode(pca9538_device_t* device, uint8_t index, pca9538_mode_t mode);
bool pca9538_write_polarity(pca9538_device_t* device, uint8_t index, bool inverted);

/**
 * pca9538_read_input() must be called to update the internal saved registers.
 */
bool pca9538_get_pin(pca9538_device_t* device, uint8_t index);
/**
 * Updates the internal saved register to the new pin value. pca9538_write_output() must be called, to update the device with new value.
 */
void pca9538_write_pin(pca9538_device_t* device, uint8_t index, bool value);
/**
 * Updates the internal saved register to the new toggled pin value. pca9538_write_output() must be called, to update the device with new value.
 */
void pca9538_toogle_pin(pca9538_device_t* device, uint8_t index);

#ifdef __cplusplus
}
#endif

#endif
