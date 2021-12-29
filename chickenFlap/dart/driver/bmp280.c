#include "../../../chickenFlap/dart/driver/bmp280.h"


bool bmp280_read_register(bmp280_device_t* device, uint8_t addr, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	if (!spi_begin(device->bus))
		return false;
	digitalWrite(device->cs, LOW);

	addr |= 0x80;

	bool ret = true;
	ret &= spi_write(device->bus, &addr, sizeof(addr));
	ret &= spi_read(device->bus, data, size);
	digitalWrite(device->cs, HIGH);
	ret &= spi_end(device->bus);
	return ret;
}

bool bmp280_write_register(bmp280_device_t* device, uint8_t addr, uint8_t data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	if (!spi_begin(device->bus))
		return false;
	digitalWrite(device->cs, LOW);

	addr &= ~0x80;

	bool ret = true;
	ret &= spi_write(device->bus, &addr, sizeof(addr));
	ret &= spi_write(device->bus, &data, sizeof(data));
	digitalWrite(device->cs, HIGH);
	ret &= spi_end(device->bus);
	return ret;
}


bool bmp280_init(bmp280_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	// Reset device
	if (!bmp280_reset(device))
		return false;

	// Check if Chip ID is correct
	uint8_t chip_id = 0;
	if (!bmp280_get_chip_id(device, &chip_id))
		return false;

	if (chip_id != BMP280_CHIP_ID)
		return false;

	// Read calibration
	if (!bmp280_read_register(device, 0x88, (uint8_t*)&device->calibration.dig_T1, sizeof(device->calibration.dig_T1)))
		return false;
	if (!bmp280_read_register(device, 0x8A, (uint8_t*)&device->calibration.dig_T2, sizeof(device->calibration.dig_T2)))
		return false;

	// Set default settings for high precision
	if (!bmp280_set_mode(device, BMP280_MODE_NORMAL))
		return false;
	if (!bmp280_set_t_standby(device, BMP280_T_STANDBY_0_5ms))
		return false;
	if (!bmp280_set_pressure_oversampling(device, BMP280_PRESSURE_OVERSAMPLING_16))
		return false;
	if (!bmp280_set_temperature_oversampling(device, BMP280_TEMPERATURE_OVERSAMPLING_8))
		return false;
	if (!bmp280_set_filter(device, BMP280_FILTER_16))
		return false;
	return true;
}

bool bmp280_reset(bmp280_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->register_config = 0;
	device->register_ctrl_meas = 0;

	return bmp280_write_register(device, BMP280_REG_RESET, BMP280_RESET_VALUE);
}

bool bmp280_get_chip_id(bmp280_device_t* device, uint8_t* id) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(id, DART_ERROR_INVALID_VALUE, false);

	*id = 0;
	return bmp280_read_register(device, BMP280_REG_ID, id, sizeof(uint8_t));
}

bool bmp280_set_mode(bmp280_device_t* device, uint8_t mode) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(mode <= BMP280_MODE_NORMAL, DART_ERROR_INVALID_VALUE, false);

	device->register_ctrl_meas &= ~BMP280_CLEAR_MODE;
	device->register_ctrl_meas |= mode;
	return bmp280_write_register(device, BMP280_REG_CONFIG, device->register_ctrl_meas);
}

bool bmp280_set_t_standby(bmp280_device_t* device, uint8_t t_standby) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(t_standby <= BMP280_T_STANDBY_4000ms, DART_ERROR_INVALID_VALUE, false);

	device->register_config &= ~(BMP280_CLEAR_T_STANDBY << 5);
	device->register_config |= t_standby << 5;
	return bmp280_write_register(device, BMP280_REG_CONFIG, device->register_config);
}

bool bmp280_set_pressure_oversampling(bmp280_device_t* device, uint8_t oversampling) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(oversampling <= BMP280_PRESSURE_OVERSAMPLING_16, DART_ERROR_INVALID_VALUE, false);

	device->register_ctrl_meas &= ~(BMP280_CLEAR_PRESSURE_OVERSAMPLING << 2);
	device->register_ctrl_meas |= oversampling << 2;
	return bmp280_write_register(device, BMP280_REG_CONFIG, device->register_ctrl_meas);
}

bool bmp280_set_temperature_oversampling(bmp280_device_t* device, uint8_t temperature) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(temperature <= BMP280_TEMPERATURE_OVERSAMPLING_16, DART_ERROR_INVALID_VALUE, false);

	device->register_ctrl_meas &= ~(BMP280_CLEAR_TEMPERATURE_OVERSAMPLING << 5);
	device->register_ctrl_meas |= temperature << 5;
	return bmp280_write_register(device, BMP280_REG_CONFIG, device->register_ctrl_meas);
}

bool bmp280_set_filter(bmp280_device_t* device, uint8_t filter) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(filter <= BMP280_FILTER_16, DART_ERROR_INVALID_VALUE, false);

	device->register_config &= ~(BMP280_CLEAR_FILTER << 2);
	device->register_config |= filter << 2;
	return bmp280_write_register(device, BMP280_REG_CONFIG, device->register_config);
}

bool bmp280_read_temperature(bmp280_device_t* device, float* temperature) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(temperature, DART_ERROR_INVALID_VALUE, false);

	int32_t adc;
	if (!bmp280_read_register(device, BMP280_REG_TEMP, (uint8_t*)&adc, 3))
		return false;

	int32_t adc_T = adc >> 4;
	int32_t var1 = ((((adc_T >> 3) - ((int32_t)device->calibration.dig_T1 << 1))) * ((int32_t)device->calibration.dig_T2)) >> 11;
	int32_t var2 = (((((adc_T >> 4) - ((int32_t)device->calibration.dig_T1)) * ((adc_T >> 4) - ((int32_t)device->calibration.dig_T1))) >> 12) * ((int32_t)device->calibration.dig_T3)) >> 14;

	// Save t_fine later for read_pressure
	device->t_fine = var1 + var2;

	// Calculate correct temperature
	float T = (device->t_fine * 5 + 128) >> 8;
	*temperature = T / 100;

	return true;
}

bool bmp280_read_pressure(bmp280_device_t* device, float* pressure) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(pressure, DART_ERROR_INVALID_VALUE, false);

	// Read temperature for t_fine
	float temperature;
	if (!bmp280_read_temperature(device, &temperature))
		return false;

	int32_t adc;
	if (!bmp280_read_register(device, BMP280_REG_PRESS, (uint8_t*)&adc, 3))
		return false;

	int32_t adc_P = adc >> 4;
	int64_t var1 = ((int64_t)device->t_fine) - 128000;
	int64_t var2 = var1 * var1 * (int64_t)device->calibration.dig_P6;
	var2 = var2 + ((var1 * (int64_t)device->calibration.dig_P5) << 17);
	var2 = var2 + (((int64_t)device->calibration.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)device->calibration.dig_P3) >> 8) + ((var1 * (int64_t)device->calibration.dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)device->calibration.dig_P1) >> 33;

	if (var1 == 0) {
		return 0;
	}
	int64_t p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)device->calibration.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)device->calibration.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)device->calibration.dig_P7) << 4);
	*pressure = (float)p / 256;

	return true;
}
