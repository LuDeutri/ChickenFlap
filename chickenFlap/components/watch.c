#include "watch.h"

void watch_init(){
	watchRestart = true;
	watch.watchTimer = 0;
}

void watch_update(){
	// watch start condition
	if (watchRestart){
		watch.watchDecHour = 0;
		watch.watchOneHour = 0;
		watch.watchDecMinute = 0;
		watch.watchOneMinute = 0;
		watchRestart = false;
	}

	// Count watch
	// Increase one-minute every 60,000ms
	if (millis() > watch.watchTimer + 60000){
		watch.watchTimer = millis();
		watch.watchOneMinute++;
	}

	// Increase dec-minute if one-minute reaches 10 and set one-minute back to 0
	if (watch.watchOneMinute >= 10) {
		watch.watchDecMinute++;
		watch.watchOneMinute = 0;
	}

	// Increase one-hour if dec-minute reaches 6 and set dec-minute back to 0
	if (watch.watchDecMinute >= 6) {
		watch.watchOneHour++;
		watch.watchDecMinute = 0;
	}

	// Increase dec-hour if one-hour reaches 10 and set one-hour back to 0
	if (watch.watchOneHour >= 10) {
		watch.watchDecHour++;
		watch.watchOneHour = 0;
	}

	// Restart if 24h is reached. Except when the time is set by the end-user
	if (watch.watchDecHour >= 2 && watch.watchOneHour >= 4 && (display.watchSelect == WATCH_SELECT_NONE))
		watchRestart = true;
}

void resetSecCounter(){
	watch.watchTimer = millis();
}
