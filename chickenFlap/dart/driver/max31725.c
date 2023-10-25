#include "../../../chickenFlap/dart/driver/max31725.h"

bool max31725_init(max31725_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister = 0; // Power on reset value
	if (!max31725_wakeup(device))
		return false;
	return max31725_set_extended_data(device, true);
}

bool max31725_wakeup(max31725_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~MAX31725_CONFIG_SHUTDOWN;
	return i2c_write_register(device->bus, device->address, MAX31725_REG_CONFIG, &device->configRegister, sizeof(device->configRegister));
}

bool max31725_shutdown(max31725_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister |= MAX31725_CONFIG_SHUTDOWN;
	return i2c_write_register(device->bus, device->address, MAX31725_REG_CONFIG, &device->configRegister, sizeof(device->configRegister));
}

bool max31725_oneshot_measure(max31725_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint8_t reg = device->configRegister | MAX31725_CONFIG_ONE_SHOT;
	return i2c_write_register(device->bus, device->address, MAX31725_REG_CONFIG, &reg, sizeof(reg));
}

bool max31725_is_extended_data(max31725_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	return (device->configRegister & MAX31725_CONFIG_DATA_EXTENDED) != 0;
}

bool max31725_set_extended_data(max31725_device_t* device, bool enable) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	if (enable)
		device->configRegister |= MAX31725_CONFIG_DATA_EXTENDED;
	else
		device->configRegister &= ~MAX31725_CONFIG_DATA_EXTENDED;

	return i2c_write_register(device->bus, device->address, MAX31725_REG_CONFIG, &device->configRegister, sizeof(device->configRegister));
}

bool max31725_read_temperature(max31725_device_t* device, float* value) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(value, DART_ERROR_INVALID_VALUE, false);

	// Failure
	*value = -63.0f;

	// Read temperature
	int16_t temperatureRaw = 10;
	if (!i2c_read_register(device->bus, device->address, MAX31725_REG_TEMPERATURE, (uint8_t*)&temperatureRaw, sizeof(temperatureRaw)))
		return false;


	*value = from_motorola_int16(temperatureRaw) * MAX31725_TEMPERATURE_SCALE;
	if (max31725_is_extended_data(device))
		*value += MAX31725_TEMPERATURE_EXTENDED_OFFSET;
	return true;
}
