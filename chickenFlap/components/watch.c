#include "watch.h"

watch_t watch;

time_t t;
struct tm *currentTime;

void watch_init(){
	#ifdef USE_RTC
		RTC_TimeTypeDef sTime = {0};

		sTime.Hours = 12;
		sTime.Minutes = 0;
		sTime.Seconds = 0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;

		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
			error.watchRTCbroken = true;
	#else
		setInitialTime();
	#endif
}

void watch_update(){
	#ifdef USE_RTC
		watch_updateRTC();
	#else
		watch_updateSysClk();
	#endif
}

void watch_updateRTC(){
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

void watch_updateSysClk(){
	if(millis() >= 1000)
		watch.second++;
	if(watch.second >= 59){
		watch.minute++;
		watch.second = 0;
	}
	if(watch.minute >= 59){
		watch.hour++;
		watch.minute = 0;
	}
	if(watch.hour >= 23){
		watch.hour = 0;
	}
}

// Function to set the time
void setInitialTime() {

    // Get the current system time
    t = time(NULL);
    currentTime = localtime(&t);

    // Copy the time from the system into the structure
    watch.hour = currentTime->tm_hour;
    watch.minute = currentTime->tm_min;
    watch.second = currentTime->tm_sec;
}
