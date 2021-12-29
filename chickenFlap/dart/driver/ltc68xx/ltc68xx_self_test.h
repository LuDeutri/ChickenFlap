#ifndef DART_DRIVER_LTC68XX_LTC68XX_SELF_TEST_H_
#define DART_DRIVER_LTC68XX_LTC68XX_SELF_TEST_H_

#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_data.h"
#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t ltc68xx_get_self_test_expected_result(ltc68xx_adc_mode_t adcMode, uint8_t selfTestMode);
bool ltc68xx_self_test_cell_voltages(ltc68xx_chain_t* chain, uint8_t selfTestMode);
bool ltc68xx_self_test_aux_voltages(ltc68xx_chain_t* chain, uint8_t selfTestMode);
bool ltc68xx_self_test_status_voltages(ltc68xx_chain_t* chain, uint8_t selfTestMode);
bool ltc68xx_self_test_open_wire(ltc68xx_chain_t* chain);

#ifdef __cplusplus
}
#endif

#endif
