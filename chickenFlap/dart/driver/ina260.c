#include "../../../chickenFlap/dart/driver/ina260.h"

bool ina260_read_register(ina260_device_t* device, uint8_t reg, uint16_t* value) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(value, DART_ERROR_INVALID_VALUE, false);

	uint16_t motorolaVal = 0;
	bool res = i2c_read_register(device->bus, device->device, reg, (uint8_t*)&motorolaVal, sizeof(motorolaVal));

	// Convert
	*value = from_motorola_uint16(motorolaVal);
	return res;
}

bool ina260_write_register(ina260_device_t* device, uint8_t reg, uint16_t value) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint16_t motorolaVal = to_motorola_uint16(value);
	return i2c_write_register(device->bus, device->device, reg, (uint8_t*)&motorolaVal, sizeof(motorolaVal));
}

bool ina260_init(ina260_device_t* device) {
	if (!ina260_reset(device))
		return false;

	device->enableCurrentFilter = false;
	device->enableVoltageFilter = false;

	uint16_t manufacturer_id = 0;
	if (!ina260_read_manufacturer_id(device, &manufacturer_id))
		return false;

	if (manufacturer_id != 0x5449)
		return false;

	if (!ina260_write_mode(device, INA260_CONFIG_MODE_CURRENT_VOLTAGE_CONT))
		return false;
	if (!ina260_write_averaging(device, INA260_CONFIG_AVG_4))
		return false;
	if (!ina260_write_bus_voltage_conversion_time(device, INA260_CONFIG_VBUSCT_588uS))
		return false;
	return ina260_write_current_conversion_time(device, INA260_CONFIG_ISHCT_588uS);
}

bool ina260_reset(ina260_device_t* device) {
 	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->lastCurrent = 0;
	device->lastVoltage = 0;

	bool ret = ina260_write_register(device, INA260_REG_CONFIG, INA260_CONFIG_RESET);
	if (ret) {
		device->configRegister = INA260_CONFIG_AVG_16 | INA260_CONFIG_VBUSCT_1_1ms | INA260_CONFIG_ISHCT_1_1ms | INA260_CONFIG_MODE_CURRENT_VOLTAGE_CONT;
		device->maskEnableRegister = 0;
	}
	return ret;
}

bool ina260_enable_voltage_filter(ina260_device_t* device, bool enable) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	device->enableVoltageFilter = enable;
	return true;
}

bool ina260_enable_current_filter(ina260_device_t* device, bool enable) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	device->enableCurrentFilter = enable;
	return true;
}

bool ina260_power_down(ina260_device_t* device) {
	return ina260_write_mode(device, INA260_CONFIG_MODE_POWER_DOWN);
}

bool ina260_read_manufacturer_id(ina260_device_t* device, uint16_t* value) {
	return ina260_read_register(device, INA260_REG_MANUFACTURER_ID, value);
}

bool ina260_read_device_id(ina260_device_t* device, uint16_t* value) {
	uint16_t val = 0;
	bool ret = ina260_read_register(device, INA260_REG_DIE_ID, &val);
	*value = (val >> 4);
	return ret;
}

bool ina260_read_die_revision(ina260_device_t* device, uint16_t* value) {
	uint16_t val = 0;
	bool ret = ina260_read_register(device, INA260_REG_DIE_ID, &val);
	*value = (val & 0xFF);
	return ret;
}

bool ina260_read_current(ina260_device_t* device, float* value) {
	int16_t val = 0;
	bool ret = ina260_read_register(device, INA260_REG_CURRENT, (uint16_t*)(&val));
	if (!ret) {
		*value = 0.0f;
		return false;
	}
	float newValue = val * INA260_CURRENT_SCALE;
	if (device->enableCurrentFilter)
		device->lastCurrent = lowPassFilter(device->lastCurrent, newValue, 0.5f);
	else
		device->lastCurrent = newValue;
	*value = device->lastCurrent;
	return ret;
}

bool ina260_read_bus_voltage(ina260_device_t* device, float* value) {
	uint16_t val = 0;
	bool ret = ina260_read_register(device, INA260_REG_BUS_VOLTAGE, &val);
	if (!ret) {
		*value = 0.0f;
		return false;
	}

	float newValue = val * INA260_VOLTAGE_SCALE;
	if (device->enableVoltageFilter)
		device->lastVoltage = lowPassFilter(device->lastVoltage, newValue, 0.5f);
	else
		device->lastVoltage = newValue;
	*value = device->lastVoltage;
	return ret;
}

bool ina260_read_power(ina260_device_t* device, float* value) {
	uint16_t val = 0;
	bool ret = ina260_read_register(device, INA260_REG_POWER, &val);
	*value = val * INA260_POWER_SCALE;
	return ret;
}

bool ina260_write_mode(ina260_device_t* device, uint16_t mode) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_MODE;
	device->configRegister |= mode;
	return ina260_write_register(device, INA260_REG_CONFIG, device->configRegister);
}

bool ina260_write_averaging(ina260_device_t* device, uint16_t averaging) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_AVG;
	device->configRegister |= averaging;
	return ina260_write_register(device, INA260_REG_CONFIG, device->configRegister);
}

bool ina260_write_bus_voltage_conversion_time(ina260_device_t* device, uint16_t time) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_VBUST_CT;
	device->configRegister |= time;
	return ina260_write_register(device, INA260_REG_CONFIG, device->configRegister);
}

bool ina260_write_current_conversion_time(ina260_device_t* device, uint16_t time) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ISHCT;
	device->configRegister |= time;
	return ina260_write_register(device, INA260_REG_CONFIG, device->configRegister);
}

bool ina260_set_alert_current_over(ina260_device_t* device, float current) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ALERT;
	device->configRegister |= (1u << 15);
	if (!ina260_write_register(device, INA260_REG_CONFIG, device->configRegister))
		return false;

	int16_t val = (int16_t)(current / INA260_CURRENT_SCALE);
	return ina260_write_register(device, INA260_REG_ALERT_LIMIT, val);
}

bool ina260_set_alert_current_under(ina260_device_t* device, float current) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ALERT;
	device->configRegister |= (1u << 14);
	if (!ina260_write_register(device, INA260_REG_CONFIG, device->configRegister))
		return false;

	int16_t val = (int16_t)(current / INA260_CURRENT_SCALE);
	return ina260_write_register(device, INA260_REG_ALERT_LIMIT, val);
}

bool ina260_set_alert_voltage_over(ina260_device_t* device, float voltage) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(voltage > 0, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ALERT;
	device->configRegister |= (1u << 13);
	if (!ina260_write_register(device, INA260_REG_CONFIG, device->configRegister))
		return false;

	uint16_t val = (uint16_t)(voltage / INA260_VOLTAGE_SCALE);
	return ina260_write_register(device, INA260_REG_ALERT_LIMIT, val);
}

bool ina260_set_alert_voltage_under(ina260_device_t* device, float voltage) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(voltage > 0, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ALERT;
	device->configRegister |= (1u << 12);
	if (!ina260_write_register(device, INA260_REG_CONFIG, device->configRegister))
		return false;

	uint16_t val = (uint16_t)(voltage / INA260_VOLTAGE_SCALE);
	return ina260_write_register(device, INA260_REG_ALERT_LIMIT, val);
}

bool ina260_set_alert_power_over(ina260_device_t* device, float power) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(power > 0, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ALERT;
	device->configRegister |= (1u << 11);
	if (!ina260_write_register(device, INA260_REG_CONFIG, device->configRegister))
		return false;

	uint16_t val = (uint16_t)(power / INA260_POWER_SCALE);
	return ina260_write_register(device, INA260_REG_ALERT_LIMIT, val);
}

bool ina260_set_alert_conversion_ready(ina260_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->configRegister &= ~INA260_CLEAR_ALERT;
	device->configRegister |= (1u << 10);
	return ina260_write_register(device, INA260_REG_CONFIG, device->configRegister);
}
