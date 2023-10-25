#include "../../../chickenFlap/dart/util/fade.h"

float fade_led(util_time_t t) {
	util_time_t time = millis() % t;

	float v = time / (float)t;
	if (v >= 0.5f)
		v = 1 - v;
	return v  * 2;
}
