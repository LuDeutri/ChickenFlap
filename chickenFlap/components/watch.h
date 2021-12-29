/*
 * This is a clock implemented in software
 */

#ifndef COMPONENTS_WATCH_H_
#define COMPONENTS_WATCH_H_

#include "display.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef struct{
	uint8_t watchDecHour;
	uint8_t watchOneHour;
	uint8_t watchDecMinute;
	uint8_t watchOneMinute;
	util_time_t watchTimer;
} watch_t;
watch_t watch;

bool watchRestart;

void watch_init();
void watch_update();


#endif
