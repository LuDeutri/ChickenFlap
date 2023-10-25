/*
 * This is a clock implemented in software
 */

#ifndef COMPONENTS_WATCH_H_
#define COMPONENTS_WATCH_H_

#include "display.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef struct{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} watch_t;
extern watch_t watch;


void watch_init();
void watch_update();

void watch_setTime(uint8_t h, uint8_t m, uint8_t s);

#endif
