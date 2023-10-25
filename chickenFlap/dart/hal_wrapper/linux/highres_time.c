#include "../../../../chickenFlap/dart/hal_wrapper/linux/highres_time.h"

#if defined(USE_LINUX) && defined(HAL_ENABLE_HIGHRES_TIME)
void highres_time_init() {

}

uint64_t highres_time_get() {
	return chrono_time_highres();
}
#endif
