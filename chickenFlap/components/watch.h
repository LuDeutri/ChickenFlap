/*
 * This is a clock implemented in software
 */

#ifndef COMPONENTS_WATCH_H_
#define COMPONENTS_WATCH_H_

#include "display.h"
#include "time.h"
#include "../dart/hal_wrapper/hal_wrapper.h"
#include "DS3231RealTimeClkModule.h"

typedef struct{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} watch_t;
extern watch_t watch;


void watch_init();
void watch_update();

/*
 * Set time with system time. Used in init if RTC is not used.
 */
void setInitialTime();

/*
 * Update RTC time. USE_RTC must be defined in config.h
 */
void watch_updateRTC();

/*
 * Update watch struct with system clock. This method is used if USE_RTC
 * is not defined in config.h
 */
void watch_updateSysClk();

/*
 * Set time with given values
 * @param h - hours
 * @param m - minutes
 * @param s - seconds
 */
void watch_setTime(uint8_t h, uint8_t m, uint8_t s);


#endif
