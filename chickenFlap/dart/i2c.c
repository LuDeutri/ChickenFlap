#include "../../chickenFlap/dart/i2c.h"

void dartI2C_init() {
	hal_i2c_init();
}

void dartI2C_update() {
	hal_i2c_update();
}

void i2c_begin(int i2c) {
	DART_ASSERT(i2c >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(i2c < I2C_COUNT, DART_ERROR_INVALID_VALUE);
	hal_i2c_begin(i2c);
}

void i2c_end(int i2c) {
	DART_ASSERT(i2c >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(i2c < I2C_COUNT, DART_ERROR_INVALID_VALUE);
	hal_i2c_end(i2c);
}

bool i2c_write(int i2c, uint8_t address, uint8_t* data, size_t size) {
	DART_ASSERT_RETURN(i2c >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(i2c < I2C_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(address < I2C_MAX_SLAVE_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size > 0, DART_ERROR_INVALID_VALUE, false);

	return hal_i2c_write(i2c, address, data, size);
}

bool i2c_read(int i2c, uint8_t address, uint8_t* data, size_t size) {
	DART_ASSERT_RETURN(i2c >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(i2c < I2C_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(address < I2C_MAX_SLAVE_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size > 0, DART_ERROR_INVALID_VALUE, false);

	return hal_i2c_read(i2c, address, data, size);
}

bool i2c_slave_ready(int i2c, uint8_t address) {
	DART_ASSERT_RETURN(i2c >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(i2c < I2C_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(address < I2C_MAX_SLAVE_COUNT, DART_ERROR_INVALID_VALUE, false);

	return hal_i2c_slave_ready(i2c, address);
}

bool i2c_write_register(int i2c, uint8_t address, uint8_t regAddr, uint8_t* data, size_t size) {
	i2c_begin(i2c);

	// Prepare data sent to the slave
	uint8_t buffer[32];
	DART_ASSERT_RETURN(size + 1 <= sizeof(buffer), DART_ERROR_INVALID_VALUE, false);
	buffer[0] = regAddr;
	memcpy(buffer + 1, data, size);

	bool result = i2c_write(i2c, address, buffer, size + 1);
	i2c_end(i2c);
	return result;
}

bool i2c_read_register(int i2c, uint8_t address, uint8_t regAddr, uint8_t* data, size_t size) {
	i2c_begin(i2c);

	// Sent register we want to read to the slave
	bool result = i2c_write(i2c, address, &regAddr, sizeof(regAddr));

	// Read data back
	if (result)
		result = i2c_read(i2c, address, data, size);

	i2c_end(i2c);
	return result;
}
