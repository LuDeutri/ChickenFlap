#include "../../../../chickenFlap/dart/hal_wrapper/stm32/realTimeClock.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_RTC)

void dartRTC_init() {
	// Done by ST HAL
}

void dartRTC_update() {
	// Do nothing
}

datetime_t dartRTC_getDateTime() {
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	datetime_t out;
	out.year = 2000;

	DART_ASSERT_HAL_OK_RETURN(HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN), out);
	DART_ASSERT_HAL_OK_RETURN(HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN), out);

	out.year = date.Year + STM32_RTC_YEAR_OFFSET;
	out.month = date.Month;
	out.day = date.Date;
	out.hours = time.Hours;
	out.minutes = time.Minutes;
	out.seconds = time.Seconds;
	return out;
}

void dartRTC_setDateTime(datetime_t datetime, bool summerTime) {
	DART_ASSERT(datetime.year >= 2000 && datetime.year <= 2099, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.month >= 1 && datetime.month <= 12, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.day >= 1 && datetime.day <= 31, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.hours >= 0 && datetime.hours <= 23, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.minutes >= 0 && datetime.minutes <= 59, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(datetime.seconds >= 0 && datetime.seconds <= 59, DART_ERROR_INVALID_VALUE);

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	time.Hours = datetime.hours;
	time.Minutes = datetime.minutes;
	time.Seconds = datetime.seconds;
	time.TimeFormat = RTC_HOURFORMAT12_AM; // unused
	time.StoreOperation = RTC_STOREOPERATION_SET;

	date.WeekDay = RTC_WEEKDAY_MONDAY; // unused
	date.Date = datetime.day;
	date.Month = datetime.month;
	date.Year = datetime.year - STM32_RTC_YEAR_OFFSET;

	if (summerTime)
		time.DayLightSaving = RTC_DAYLIGHTSAVING_ADD1H;
	else
		time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;

	DART_ASSERT_HAL_OK(HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN));
	DART_ASSERT_HAL_OK(HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN));
}
#endif
