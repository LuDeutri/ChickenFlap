#include "../../../chickenFlap/dart/driver/pca9538.h"

bool pca9538_init(pca9538_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister = 0xFF; // set all pins as input
	device->polarityRegister= 0; // set all pins as not inverted
	device->inputRegister = 0;
	device->outputRegister = 0;

	if (!i2c_write_register(device->bus, device->device, PCA9538_REG_CONFIG, &device->configRegister, sizeof(device->configRegister)))
		return false;
	if (!i2c_write_register(device->bus, device->device, PCA9538_REG_POLARITY, &device->polarityRegister, sizeof(device->polarityRegister)))
		return false;
	if (!i2c_write_register(device->bus, device->device, PCA9538_REG_OUTPUT, &device->outputRegister, sizeof(device->outputRegister)))
		return false;
	return pca9538_read_input(device);
}

bool pca9538_read_input(pca9538_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	return i2c_read_register(device->bus, device->device, PCA9538_REG_INPUT, &device->inputRegister, sizeof(device->inputRegister));
}

bool pca9538_write_output(pca9538_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	return i2c_write_register(device->bus, device->device, PCA9538_REG_OUTPUT, &device->outputRegister, sizeof(device->outputRegister));
}

bool pca9538_write_mode(pca9538_device_t* device, uint8_t index, pca9538_mode_t mode) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(index < 8, DART_ERROR_INVALID_VALUE, false);

	if (mode == pca9538_mode_output)
		device->configRegister &= ~(1 << index);
	else
		device->configRegister |= (1 << index);
	return i2c_write_register(device->bus, device->device, PCA9538_REG_CONFIG, &device->configRegister, sizeof(device->configRegister));
}

bool pca9538_write_polarity(pca9538_device_t* device, uint8_t index, bool inverted) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(index < 8, DART_ERROR_INVALID_VALUE, false);

	if (inverted)
		device->polarityRegister |= (1 << index);
	else
		device->polarityRegister &= ~(1 << index);
	return i2c_write_register(device->bus, device->device, PCA9538_REG_POLARITY, &device->polarityRegister, sizeof(device->polarityRegister));
}

bool pca9538_get_pin(pca9538_device_t* device, uint8_t index) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(index < 8, DART_ERROR_INVALID_VALUE, false);

	return (device->inputRegister & (1 << index)) != 0;
}

void pca9538_write_pin(pca9538_device_t* device, uint8_t index, bool value) {
	DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(index < 8, DART_ERROR_INVALID_VALUE);

	if (value)
		device->outputRegister |= (1 << index);
	else
		device->outputRegister &= ~(1 << index);

}

void pca9538_toogle_pin(pca9538_device_t* device, uint8_t index) {
	DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(index < 8, DART_ERROR_INVALID_VALUE);

	device->outputRegister ^= (1 << index);
}
