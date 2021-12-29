#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/i2c.h"

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_I2C)
void hal_i2c_init() {
	I2C_CLOCK_LO();
	I2C_DATA_LO();

	_delay_us(1);
}

void hal_i2c_update() {
	
}

void hal_i2c_begin(int i2c) {
	// set both to high at the same time
	I2C_CLOCK_HI();
	I2C_DATA_HI();
	_delay_us(8);

	I2C_DATA_LO();
	_delay_us(8);

	I2C_CLOCK_LO();
	_delay_us(8);
}

void hal_i2c_end(int i2c) {
	I2C_DATA_LO();
	_delay_us(8);
	
	I2C_CLOCK_HI();
	_delay_us(8);

	I2C_DATA_HI();
	_delay_us(8);
}

bool hal_i2c_write(int i2c, uint8_t address, uint8_t* data, size_t size) {
	cli();
	i2c_begin(i2c);
	_hal_i2c_write(address << 1);
	for (int i = 0; i < size; i++)
		_hal_i2c_write(data[i]);
    i2c_end(i2c);
	_delay_us(10);
	sei();
	return true;
}

bool hal_i2c_read(int i2c, uint8_t address, uint8_t* data, size_t size) {
	cli();
	i2c_begin(i2c);
	_hal_i2c_write((address << 1) + 1);
	for (int i = 0; i < size; i++)
		data[i] = _hal_i2c_read(i < (size - 1));
	i2c_end(i2c);
	_delay_us(10);
	sei();
	return true;
}

void _hal_i2c_writeBit(unsigned char c)
{
	if (c > 0) {
		I2C_DATA_HI();
	}
	else {
		I2C_DATA_LO();
	}

	I2C_CLOCK_HI();
	_delay_us(8);

	I2C_CLOCK_LO();
	_delay_us(8);

	if (c > 0)
		I2C_DATA_LO();

	_delay_us(8);
}

unsigned char _hal_i2c_readBit()
{
	I2C_DATA_HI();

	I2C_CLOCK_HI();
	_delay_us(8);

	unsigned char c = I2C_DATA_PIN;

	I2C_CLOCK_LO();
	_delay_us(8);

	return (c >> I2C_DATA_PIN_NUMBER) & 1;
}

unsigned char _hal_i2c_write(unsigned char c)
{
	for (char i = 0; i < 8; i++)
	{
		_hal_i2c_writeBit(c & 128);

		c <<= 1;
	}

	return _hal_i2c_readBit();
}

unsigned char _hal_i2c_read(unsigned char ack)
{
	unsigned char res = 0;
	for (char i = 0; i < 8; i++)
	{
		res <<= 1;
		res |= _hal_i2c_readBit();
	}

	if (ack > 0)
		_hal_i2c_writeBit(0);
	else
		_hal_i2c_writeBit(1);

	_delay_us(2);
	return res;
}
#endif
