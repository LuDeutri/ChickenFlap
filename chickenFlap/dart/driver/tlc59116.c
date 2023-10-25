#include "../../../chickenFlap/dart/driver/tlc59116.h"

bool tlc59116_read_register(tlc59116_device_t* device, uint8_t reg, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	return i2c_read_register(device->bus, device->device, reg | TLC59116_AUTO_INCREMENT, data, size);
}

bool tlc59116_write_register(tlc59116_device_t* device, uint8_t reg, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	return i2c_write_register(device->bus, device->device, reg | TLC59116_AUTO_INCREMENT, data, size);
}

bool tlc59116_init(tlc59116_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->mode1 = TLC59116_MODE1_CONFIG;
	device->mode2 = TLC59116_MODE2_CONFIG;
	memset(device->ledout, 0, sizeof(device->ledout));
	if (!tlc59116_write_register(device, TLC59116_REG_MODE1, &device->mode1, sizeof(device->mode1)))
		return false;
	if (!tlc59116_write_register(device, TLC59116_REG_MODE2, &device->mode2, sizeof(device->mode2)))
		return false;

	tlc59116_disable(device);
	return true;
}

bool tlc59116_enable(tlc59116_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->mode1 &= ~TLC59116_MODE1_DISABLE_OSC;
	return tlc59116_write_register(device, TLC59116_REG_MODE1, &device->mode1, sizeof(device->mode1));
}
bool tlc59116_disable(tlc59116_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	// Disable all LEDs
	memset(device->ledout, 0, sizeof(device->ledout));

	if (!tlc59116_write_register(device, TLC59116_REG_LEDOUT0, device->ledout, sizeof(device->ledout)))
		return false;

	device->mode1 |= TLC59116_MODE1_DISABLE_OSC;
	return tlc59116_write_register(device, TLC59116_REG_MODE1, &device->mode1, sizeof(device->mode1));
}

bool tlc59116_check_and_clear_any_led_error(tlc59116_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	bool ledErrors[TLC59116_LED_COUNT];
	if (!tlc59116_read_error_flags(device, ledErrors, sizeof(ledErrors)))
		return false;

	bool anyError = false;
	for (int i = 0; i < TLC59116_LED_COUNT; i++)
		anyError |= ledErrors[i];

	if (!tlc59116_clear_error_flags(device))
		return false;
	return !anyError;
}

bool tlc59116_clear_error_flags(tlc59116_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->mode2 |= TLC59116_MODE2_CLEAR_ERR_FLAGS;
	if (!tlc59116_write_register(device, TLC59116_REG_MODE2, &device->mode2, sizeof(device->mode2)))
		return false;
	device->mode2 &= ~TLC59116_MODE2_CLEAR_ERR_FLAGS;
	return tlc59116_write_register(device, TLC59116_REG_MODE2, &device->mode2, sizeof(device->mode2));
}
bool tlc59116_read_error_flags(tlc59116_device_t* device, bool* led_errors, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(led_errors, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size == TLC59116_LED_COUNT * sizeof(bool), DART_ERROR_INVALID_VALUE, false);

	uint8_t err_flags[2];
	if (!tlc59116_read_register(device, TLC59116_REG_EFLAG1, err_flags, sizeof(err_flags)))
		return false;

	for (int i = 0; i < 8; i++)
		led_errors[i] = (err_flags[0] & (1 << i)) != 0;
	for (int i = 0; i < 8; i++)
		led_errors[i + 8] = (err_flags[1] & (1 << i)) != 0;

	return true;
}

bool tlc59116_write_led(tlc59116_device_t* device, uint8_t index, float brightness) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(index < TLC59116_LED_COUNT, DART_ERROR_INVALID_VALUE, false);

	uint8_t mode;
	if (brightness <= 0)
		mode = TLC59116_LED_MODE_OFF;
	else if (brightness >= 1)
		mode = TLC59116_LED_MODE_ON;
	else
		mode = TLC59116_LED_MODE_PWM;

	// Set mode
	uint8_t addr = index / 4;
	uint8_t shift = (index % 4) * 2;

	uint8_t value = device->ledout[addr];
	value &= ~(TLC59116_LED_MODE_CLEAR << shift);
	value |= (mode << shift);
	device->ledout[addr] = value;

	if (!tlc59116_write_register(device, TLC59116_REG_LEDOUT0 + addr, &value, sizeof(value)))
		return false;

	// Write brightness
	if (brightness <= 0 || brightness >= 1)
		return true;

	uint8_t brightnessValue = (uint8_t)(brightness * 255);
	return tlc59116_write_register(device, TLC59116_REG_PWM0 + index, &brightnessValue, sizeof(brightnessValue));
}

