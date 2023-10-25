#ifndef DART_HAL_WRAPPER_TMS570_WRAPPER_H_
#define DART_HAL_WRAPPER_TMS570_WRAPPER_H_

#include "../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

// Check if we compile for the TMS570 hardware
#ifdef USE_TMS570
	// Check if already any HAL defined
	#ifdef HAL_NAME
		#error "Already a HAL defined"
	#endif

	// Include any device specific stuff
    #include <hal_stdtypes.h>
    #include <sys_common.h>
	#include <sys_core.h>
    #include <reg_system.h>
    #include <het.h>
    #include <gio.h>
    #include <adc.h>
    #include <can.h>
    #include <spi.h>
    #include <rti.h>
    #include <lin.h>

	/**
	 * Defines the name for the HAL which is used.
	 */
	#define HAL_NAME "TMS570"

	/**
	 * HAL settings for this platform
	 */
	#define CANDATA_USE_LAX_DLC_CHECK

	#include "../../../../chickenFlap/dart/hal_wrapper/tms570/analog.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/tms570/can.h"
	#include "../../../../chickenFlap/dart/hal_wrapper/tms570/digital.h"

	#ifndef MAXFLOAT
		#define MAXFLOAT FLT_MAX
	#endif
#endif

#endif
