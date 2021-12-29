#include "../../../../chickenFlap/dart/hal_wrapper/tms570/tms570_wrapper.h"

#ifdef USE_TMS570
/**
 * Counter for the elapsed time in milliseconds. Incremented by rtiNotification and used by millis().
 */
volatile uint32_t millisCounter = 0;

/**
 * HAL Config Values set by HAL Code Generator. The following constant
 * MUST match with the values set in the HAL Code Generator.
 */

// Defines the value set for the RTI Counter 0 Clock. The value is in the unit MHz.
#define HAL_CONFIG_RTI_COUNTER_0_CLOCK 10

// Defines the value set for the RTI Compare 0 Period. The value is in the unit millisecond.
#define HAL_CONFIG_RTI_COMPARE_0_PERIOD 1

void hal_init() {
    _disable_interrupt_();

    rtiInit();

    // See HAL_CONFIG_RTI_COMPARE_0_PERIOD
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    rtiStartCounter(rtiCOUNTER_BLOCK0);

    _enable_interrupt_();
}

#ifdef HAL_ENABLE_TIME
uint32_t hal_get_millis() {
    return millisCounter;
}
#endif

void hal_yield() {

}

void hal_system_reset() {
    systemREG1->SYSECR = 0x8000;
}


size_t hal_get_free_heap() {
	return 0;
}

/**
 * RTI ISR for compare notifications. Called by HAL.
 */
void rtiNotification(uint32 notification) {
    if (notification == rtiNOTIFICATION_COMPARE0)
        millisCounter += HAL_CONFIG_RTI_COMPARE_0_PERIOD;
}

#endif
