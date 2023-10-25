#include "../../../chickenFlap/dart/util/error_delay.h"

bool error_delay(err_delay_t* delay, bool isError, util_time_t timeout) {
	DART_NOT_NULL_RETURN(delay, DART_ERROR_UTIL, true);
	if (!isError) {
		delay->last_error_time = millis();
		return false;
	}
	return (millis() - delay->last_error_time) >= timeout;
}
