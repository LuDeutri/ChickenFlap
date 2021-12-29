#include "../../../../chickenFlap/dart/hal_wrapper/dummy/realTimeClock.h"

#ifdef HAL_DUMMY_RTC

void dartRTC_init() {
	// Do nothing
}

void dartRTC_update() {
	// Do nothing
}

datetime_t dartRTC_getDateTime() {
	// Do nothing
	datetime_t out;
	memset(&out, 0, sizeof(out));
	return out;
}

void dartRTC_setDateTime(datetime_t datetime, bool summerTime) {
	DART_ASSERT(datetime.year >= 2000 && datetime.year <= 2099, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.month >= 1 && datetime.month <= 12, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.day >= 1 && datetime.day <= 31, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.hours >= 0 && datetime.hours <= 23, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.minutes >= 0 && datetime.minutes <= 59, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.seconds >= 0 && datetime.seconds <= 59, DART_ERROR_INVALID_VALUE);
	// Do nothing
}
#endif
