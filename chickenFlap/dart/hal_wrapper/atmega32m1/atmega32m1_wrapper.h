#ifndef DART_HAL_WRAPPER_ATMEGA32M1_WRAPPER_H_
#define DART_HAL_WRAPPER_ATMEGA32M1_WRAPPER_H_

#include "../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Check if we compile for the ATmega32M1 hardware
#ifdef USE_ATMEGA32M1
	// Check if already any HAL defined
	#ifdef HAL_NAME
		#error "Already a HAL defined"
	#endif

	// Include any device specific stuff
	#include <avr/interrupt.h>
	#include <avr/io.h>
	#include <avr/sleep.h>
	#include <avr/wdt.h>
	#include <avr/eeprom.h>
	#include <util/atomic.h>

	/**
	 * Defines the name for the HAL which is used.
	 */
	#define HAL_NAME "ATMEGA32M1"

	#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/analog.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/can.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/digital.h"
#endif

#endif
