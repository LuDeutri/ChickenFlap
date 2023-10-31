/*
 * The module consists of a DS3231 RTC Clock chip and Atmel AT24C32 EEPROM
 * chip. The AT24C32 has memory storage capacity of 32kB and uses the I2C bus
 * interface with 0x57 address which can be modified. It has a capability of setting
 *
 * the time and date, checking and clearing alarms and logging data with a times-*
 * tamp.
 *
 *  Created on: 28.10.2023
 *      Author: Luca Deutrich
 */

#ifndef COMPONENTS_DS3231REALTIMECLKMODULE_H_
#define COMPONENTS_DS3231REALTIMECLKMODULE_H_

#include "config.h"
#include "../dart/hal_wrapper/hal_wrapper.h"


// I2C communication address
#define I2C_ADDRESSE_DS3231 0x57

// Register addresses
#define REGISTER_CONTROL 0xE
#define REGISTER_STATUS 0xF
#define REGISTER_AGIGNG_OFFSET 0x10
#define REGISTER_TEMPUTER_UPPER_BYTE 0x11
#define REGISTER_TEMPUTER_LOWER_BYTE 0x12


#define REGISTER_SECONDS 0x00 	// MSB:0 | Bit6-4: 10-seconds | Bit3-0: seconds 	// Range 0-59
#define REGISTER_MINUTES 0x01 	// MSB:0 | Bit6-4: 10-minutes | Bit3-0: minutes 	// Range 0-59
#define REGISTER_HOURS 0x02   	// MSB:= | Bit6: 12h/24h | Bit5: 20-hour | Bit4: 10-hour | Bit3-0: hours

#define REGISTSER_DAYS 0x03 	// MSB-3:0 | 2-0: Day 					// Range: 1-7
#define REGISTER_DATE 0x4 		// MSB-Bit6: 0 | Bit5-4: 10-date | Bit3-0: date 				// Range 0-31
#define REGISTER_MONTH 0x5 		// MSB: Century | Bit6-5: 0 | Bit4: 10-month | Bit3-0: month 	// Range 0-31
#define REGISTER_YEAR 0x6 		// MSB-4: 10-year | Bit3-0: year 		// Range: 0-99


typedef enum{
	MODE_24_HOUR,
	MODE_12_HOUR
}hourMode_t;

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} ds3231_time;
extern ds3231_time timeReg;

bool ds3231_init(uint8_t i2cBus);

bool ds3231_setTime(uint8_t i2cBus, uint8_t regAddress, uint8_t *data);
bool ds3231_readTime(uint8_t i2cBus, uint8_t regAddress, uint8_t *data);



#endif
