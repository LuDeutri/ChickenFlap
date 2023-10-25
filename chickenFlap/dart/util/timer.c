#include "../../../chickenFlap/dart/util/timer.h"

util_timer_t createTimer(util_time_t period, util_time_t offset) {
	util_timer_t timer;
    timer.period = period;
    if (offset != TIMER_OFFSET_RANDOM)
    	timer.offset = offset;
    else
    	timer.offset = rand() % 1000;
    timer._lastTime = millis() + timer.offset;
    timer.first = true;
    return timer;
}

util_timer_t createTimerNoOffset(util_time_t period) {
	util_timer_t timer;
	timer.period = period;
	timer.offset = 0;
	timer._lastTime = millis();
	timer.first = true;
	return timer;
}

void resetTimer(util_timer_t* timer) {
	DART_NOT_NULL(timer, DART_ERROR_UTIL);

	timer->_lastTime = millis();
}

bool hasTimerElapsed(util_timer_t* timer) {
    DART_NOT_NULL_RETURN(timer, DART_ERROR_UTIL, false);

    if (timer->first) {
    	timer->_lastTime = millis() + timer->offset;
		timer->first = false;
		return false;
    }

    // Check if timer has elapsed
    if (!hasTimeElapsed(timer->_lastTime, timer->period))
        return false;

    resetTimer(timer);
    return true;
}
