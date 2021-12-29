#include "../../chickenFlap/dart/highres_time.h"

float highres_time_getMillis() {
	return highres_time_get() / 1000.0f;
}
