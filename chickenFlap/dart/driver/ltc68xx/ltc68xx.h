#ifndef DART_DRIVER_LTC68xx_H_
#define DART_DRIVER_LTC68xx_H_

#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_base.h"
#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_data.h"
#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_i2c.h"
#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_protocol.h"
#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_self_test.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the LTC68XX chain and sets some default parameters.
 * Returns true when the initialization was successful or false otherwise.
 */
bool ltc68xx_init(ltc68xx_chain_t* chain);

/**
 * Returns if the chain is ready (i.e. every init step and self test step is done).
 */
bool ltc68xx_is_ready(ltc68xx_chain_t* chain);

/**
 * Updates the LTC chain. This function will update the status registers, trigger the ADC conversion, read the ADCs results and check for errors.
 */
bool ltc68xx_update(ltc68xx_chain_t* chain);

/*
 * Dumps the chain of devices to the UART to help debugging.
 */
void ltc68xx_dump(ltc68xx_chain_t* chain, int uart);

#ifdef __cplusplus
}
#endif

#endif
