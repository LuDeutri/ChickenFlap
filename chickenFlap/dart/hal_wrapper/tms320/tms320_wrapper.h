#ifndef DART_HAL_WRAPPER_TMS320_WRAPPER_H_
#define DART_HAL_WRAPPER_TMS320_WRAPPER_H_

#include "../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Check if we compile for the TMS320 hardware
#ifdef USE_TMS320
	// Check if already any HAL defined
	#ifdef HAL_NAME
		#error "Already a HAL defined"
	#endif

	// Include any device specific stuff
    #include "driverlib.h"
    #include "device.h"

    /**
     * Fixes broken defines.
     */
    #ifndef INT32_MAX
        #define INT32_MAX   0x7fffffff
    #endif

    #ifndef UINT32_MAX
        #define UINT32_MAX  0xffffffff
    #endif

    #define DEVICE_HAS_FAKE_INT8

	/**
	 * Defines the name for the HAL which is used.
	 */
	#define HAL_NAME "TMS320"

	#include "../../../../chickenFlap/dart/hal_wrapper/tms320/analog.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/tms320/can.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/tms320/digital.h"
#endif

#endif
