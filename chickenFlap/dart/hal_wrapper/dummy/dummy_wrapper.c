#include "../../../../chickenFlap/dart/hal_wrapper/dummy/dummy_wrapper.h"

#ifdef HAL_DUMMY_TIME
uint32_t hal_getMillis() {
	return 0;
}
#endif
