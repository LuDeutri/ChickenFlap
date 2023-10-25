#include "watch.h"

watch_t watch;

void watch_init(){
	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = 12;
	sTime.Minutes = 0;
	sTime.Seconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
		error.watchRTCbroken = true;
}


void watch_update(){
    RTC_TimeTypeDef getTime = {0};
    RTC_DateTypeDef getDate = {0};

    if (HAL_RTC_GetTime(&hrtc, &getTime, RTC_FORMAT_BCD) != HAL_OK)
      error.watchRTCbroken = true;

    // getDate is required to synchronize getTime, even if the date is not used
    HAL_RTC_GetDate(&hrtc, &getDate, RTC_FORMAT_BIN);


    watch.hour = getTime.Hours;
    watch.minute = getTime.Minutes;
    watch.second = getTime.Seconds;
}

void watch_setTime(uint8_t h, uint8_t m, uint8_t s){
	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = h;
	sTime.Minutes = m;
	sTime.Seconds = s;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
		error.watchRTCbroken = true;
}
