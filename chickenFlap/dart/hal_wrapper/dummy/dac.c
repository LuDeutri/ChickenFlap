#include "../../../../chickenFlap/dart/hal_wrapper/dummy/dac.h"

#ifdef HAL_DUMMY_GPIO

void dartDAC_init() {
	// Do nothing
}

void dartDAC_update() {
    // Do nothing
}

bool dacWrite(uint8_t channel, uint32_t data){
	DART_ASSERT_RETURN(data <= DAC_MAX_VALUE, DART_ERROR_INVALID_VALUE, false);

	return false;
}
#endif
