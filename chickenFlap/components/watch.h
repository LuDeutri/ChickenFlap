/*
 * This is a clock implemented in software
 */

#ifndef COMPONENTS_WATCH_H_
#define COMPONENTS_WATCH_H_

#include "display.h"
#include "time.h"
#include "../dart/hal_wrapper/hal_wrapper.h"
#include "../DS3231/DS3231RealTimeClkModule.h"

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayOfWeek;
	uint8_t dayOfMonth;
	uint8_t month;
	uint8_t year;
} watch_t;
extern watch_t watch;

// If DS3231 is used, temperature is measured too
extern float temperature;

/*
 * Init clk depending on configuration in config.h
 * 	- uC's RTC
 * 	- RTC module DS3132
 * 	- Sys clk
 */
void watch_init();

/*
 * Update clk depending on configuration in config.h
 * 	- uC's RTC
 * 	- RTC module DS3132
 * 	- Sys clk
 */
void watch_update();

/*
 * Set time with given values
 * @param h - hours
 * @param m - minutes
 * @param s - seconds
 */
void watch_setTime(uint8_t h, uint8_t m, uint8_t s);


#endif
