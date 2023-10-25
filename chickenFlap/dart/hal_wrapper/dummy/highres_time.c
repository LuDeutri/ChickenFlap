#include "../../../../chickenFlap/dart/hal_wrapper/dummy/highres_time.h"

#ifdef HAL_DUMMY_HIGHRES_TIME
void highres_time_init() {
	// Do nothing
}

uint64_t highres_time_get() {
	return millis() * 1000;
}
#endif
