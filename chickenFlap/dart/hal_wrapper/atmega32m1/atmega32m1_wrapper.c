#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/atmega32m1_wrapper.h"

#ifdef USE_ATMEGA32M1
volatile int32_t hal_timerCounter = 0;

void hal_init() {
	// Set timer 0 (8 MHz Clock), CTC
	TCNT0 = 0;
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 124;
}

#ifdef HAL_ENABLE_TIME
uint32_t hal_getMillis() {
	uint32_t t = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		t = hal_timerCounter;
	}
	return t;
}
#endif

void hal_yield() {

}

void hal_system_reset() {
	cli();
	wdt_enable(WDTO_15MS);
	while(1);
}

ISR(TIMER0_COMPA_vect)
{
	// called every millisecond
	hal_timerCounter++;
}

size_t hal_get_free_heap() {
	return 0;
}

#endif
