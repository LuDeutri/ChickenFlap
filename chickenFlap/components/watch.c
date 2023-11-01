#include "watch.h"

watch_t watch;
uint32_t actuallyTime;
float temperature;

time_t t;
struct tm *currentTime;



void watch_init(){
	temperature = 0;
	actuallyTime = 0;

	// -------------------- uC RTC ----------------------
	#ifdef USE_RTC
		RTC_TimeTypeDef sTime = {0};

		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;

		watch_setTimeRTC(12, 0, 0); // Initial time 12:00 o'clock


	// ---------------- DS3231 module RTC ----------------
	#elif defined(USE_DS3231)
		if(ds3231_init())
			error.watchError = true;

	// ----------------- System clk ---------------------
	#else
		// Get the current system time
		t = time(NULL);
		currentTime = localtime(&t);

		// Copy the time from the system into the structure
		watch.hour = currentTime->tm_hour;
		watch.minute = currentTime->tm_min;
		watch.second = currentTime->tm_sec;
	#endif
}

void watch_update(){
	// -------------------- uC RTC ----------------------
	#ifdef USE_RTC
		RTC_TimeTypeDef getTime = {0};
		RTC_DateTypeDef getDate = {0};

		if (HAL_RTC_GetTime(&hrtc, &getTime, RTC_FORMAT_BCD) != HAL_OK)
		  error.watchError = true;

		// getDate is required to synchronize getTime, even if the date is not used
		HAL_RTC_GetDate(&hrtc, &getDate, RTC_FORMAT_BIN);


		watch.hour = getTime.Hours;
		watch.minute = getTime.Minutes;
		watch.second = getTime.Seconds;

	// ---------------- DS3231 module RTC ----------------
	#elif defined(USE_DS3231)
		if(!ds3231_readTime())
			error.watchError = true;

		watch.second = timeReg.second;
		watch.minute = timeReg.minute;
		watch.hour = timeReg.hour;
		watch.dayOfWeek = timeReg.dayOfWeek;
		watch.dayOfMonth = timeReg.dayOfMonth;
		watch.month = timeReg.month;
		watch.year = timeReg.year;

		if(!ds3231_readTemp(&temperature));
			// Not system critical, do nothing


	// ----------------- System clk ---------------------
	#else
		if(millis() - actuallyTime > 1000){ // Seconds
			actuallyTime = millis();
			watch.second++;
		}
		if(watch.second >= 59){		// Minutes
			watch.minute++;
			watch.second = 0;
		}
		if(watch.minute >= 59){		// Hours
			watch.hour++;
			watch.minute = 0;
		}
		if(watch.hour >= 24){		// Reset hours after day end
			watch.hour = 0;
		}
	#endif

}

void watch_setTime(uint8_t h, uint8_t m, uint8_t s){
	// Stop if invalid parameters are given
	if (h > 23 || m > 59 || s > 59)
		return;

	// -------------------- uC RTC ----------------------
	#ifdef USE_RTC
		RTC_TimeTypeDef sTime = {0};

		sTime.Hours = h;
		sTime.Minutes = m;
		sTime.Seconds = s;

		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
			error.watchError = true;

	// ---------------- DS3231 module RTC ----------------
	#elif defined(USE_DS3231)
		ds3231_setTimeShort(h, m, s);

	// ----------------- System clk ---------------------
	#else
		watch.hour = h;
		watch.minute = m;
		watch.second = s;

	#endif
}
