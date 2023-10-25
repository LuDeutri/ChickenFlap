#include "../../../chickenFlap/dart/util/time.h"

#ifdef HAL_ENABLE_TIME_SCALE
float millis_scale = 1.0f;
#endif

util_time_t millis() {
#ifdef HAL_ENABLE_TIME_SCALE
    return (util_time_t)floorf(hal_get_millis() * millis_scale);
#else
    return hal_get_millis();
#endif
}

bool hasTimeElapsed(uint32_t start, uint32_t period) {
    uint32_t time = millis();
    if (time < start) // Check for overflow
        return true;
    return (time - start) >= period;
}

void delayMillis(uint32_t time) {
    uint32_t start = millis();
    while (!hasTimeElapsed(start, time)); // busy-wait
}

void millisSetScale(float scale) {
#ifdef HAL_ENABLE_TIME_SCALE
    if (scale >= 0.001f)
        millis_scale = scale;
#else
    DART_NOT_USED(scale);
#endif
}
