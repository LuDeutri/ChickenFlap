#include "../../../../chickenFlap/dart/hal_wrapper/tms320/tms320_wrapper.h"

#ifdef USE_TMS320
void hal_init() {
    Device_init();

    Interrupt_initModule();
    Interrupt_initVectorTable();
    EINT;
    ERTM;
}

#ifdef HAL_ENABLE_TIME
uint32_t hal_getMillis() {
    return 0;
}
#endif

void hal_yield() {

}

void hal_system_reset() {

}


size_t hal_get_free_heap() {
	return 0;
}

#endif
