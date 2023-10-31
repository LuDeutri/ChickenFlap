#include "DS3231RealTimeClkModule.h"

ds3231_time timeReg;

bool ds3231_init(uint8_t i2cBus){
	bool tmp = true;

	uint8_t configuration = 0x1C;
	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_CONTROL, &buffer, sizeof(buffer));

	// Initial time
	uint8_t initialTimeHour = 12;
	uint8_t initialTimeMinute = 0;
	uint8_t initialTimeSecond = 0;
	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_HOURS, initialTimeHour, sizeof(initialTimeHour));
	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_MINUTES, initialTimeMinute, sizeof(initialTimeMinute));
	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_SECONDS, initialTimeSecond, sizeof(initialTimeSecond));

	return tmp;
}

bool ds3231_setTime(uint8_t i2cBus, uint8_t h, uint8_t m, uint8_t s){
	bool tmp = true;

	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_SECONDS, h, sizeof(h));
	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_MINUTES, m, sizeof(m));
	tmp &= i2c_write_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_HOURS, s, sizeof(s));

	return tmp;
}

bool ds3231_readTime(uint8_t i2cBus){
	bool tmp = true;
	tmp &= i2c_read_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_SECONDS, &timeReg.second, sizeof(timeReg.second));
	tmp &= i2c_read_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_MINUTES, &timeReg.minute, sizeof(timeReg.minute));
	tmp &= i2c_read_register(i2cBus, I2C_ADDRESSE_DS3231, REGISTER_HOURS, &timeReg.hour, sizeof(timeReg.hour));

	return tmp;
}

