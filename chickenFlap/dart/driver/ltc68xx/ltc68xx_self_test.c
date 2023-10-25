#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_self_test.h"

uint16_t ltc68xx_get_self_test_expected_result(ltc68xx_adc_mode_t adcMode, uint8_t selfTestMode) {
    if (selfTestMode == 1) {
        if (adcMode == LTC68XX_ADC_27kHz)
            return 0x9565u;
        else if (adcMode == LTC68XX_ADC_14kHz)
            return 0x9553u;
        else
            return 0x9555;
    }
    else if (selfTestMode == 2) {
        if (adcMode == LTC68XX_ADC_27kHz)
            return 0x6A9Au;
        else if (adcMode == LTC68XX_ADC_14kHz)
            return 0x6AACu;
        else
            return 0x6AAAu;
    }
    return 0;
}

bool ltc68xx_self_test_cell_voltages(ltc68xx_chain_t* chain, uint8_t selfTestMode) {
    // Perform self test
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRCELL))
        return false;
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd_CVST(chain, chain->adc_mode, selfTestMode))
        return false;

    // Wait till operation is done
    delayMillis(ltc68xx_get_adc_time(chain) * 4);
    if (!ltc68xx_read_cell_voltages(chain))
        return false;

    // Find correct result (intel/motorola conversion already done by ltc68xx_get_raw_cell_voltage)
    uint16_t expectedResult = to_intel_uint16(ltc68xx_get_self_test_expected_result(chain->adc_mode, selfTestMode));

    // Check result
    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

        for (int j = 0; j < ltc68xx_get_max_cell_count(*device); j++) {
            if (ltc68xx_get_raw_cell_voltage(*device, j) != expectedResult) {
                device->error.self_test_cell_voltages_fail = true;
                break;
            }
        }
    }

    // Clear again
    return ltc68xx_cmd(chain, LTC68XX_CMD_CLRCELL);
}

bool ltc68xx_self_test_aux_voltages(ltc68xx_chain_t* chain, uint8_t selfTestMode) {
    // Perform self test
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRAUX))
        return false;
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd_AXST(chain, chain->adc_mode, selfTestMode))
        return false;

    // Wait till operation is done
    delayMillis(ltc68xx_get_adc_time(chain) * 4);
    if (!ltc68xx_read_aux(chain))
        return false;

    // Find correct result
    uint16_t expectedResult = to_intel_uint16(ltc68xx_get_self_test_expected_result(chain->adc_mode, selfTestMode));

    // Check result
    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

        device->error.self_test_aux_voltages_fail = (device->aux_a.G1V != expectedResult)
                || (device->aux_a.G2V != expectedResult)
                || (device->aux_a.G3V != expectedResult)
                || (device->aux_b.G4V != expectedResult)
                || (device->aux_b.G5V != expectedResult)
                || (device->aux_b.REF != expectedResult);
    }

    // Clear again
    return ltc68xx_cmd(chain, LTC68XX_CMD_CLRAUX);
}

bool ltc68xx_self_test_status_voltages(ltc68xx_chain_t* chain, uint8_t selfTestMode) {
    // Perform self test
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRSTAT))
        return false;
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd_STATST(chain, chain->adc_mode, selfTestMode))
        return false;

    // Wait till operation is done
    delayMillis(ltc68xx_get_adc_time(chain) * 4);
    if (!ltc68xx_read_status(chain))
        return false;

    // Find correct result
    uint16_t expectedResult = to_intel_uint16(ltc68xx_get_self_test_expected_result(chain->adc_mode, selfTestMode));

    // Check result
    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

        device->error.self_test_status_fail = (device->status_a.SC != expectedResult)
                || (device->status_a.ITMP != expectedResult)
                || (device->status_a.VA != expectedResult)
                || (device->status_b.VD != expectedResult);
    }

    // Clear again
    return ltc68xx_cmd(chain, LTC68XX_CMD_CLRSTAT);
}

bool ltc68xx_self_test_open_wire(ltc68xx_chain_t* chain) {
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRCELL))
        return false;

    // Run the ADOW commands five times to charge any capacitance
    const int numADOWCommands = 5;

    // Pull-up mode
    for (int i = 0; i < numADOWCommands; i++)
        if (!ltc68xx_cmd_ADOW(chain, chain->adc_mode, true, chain->discharge_mode == DISCHARGE_WHILE_CONVERSION, LTC68XX_ADCV_ALL_CELLS))
            return false;

    delayMillis(ltc68xx_get_adc_time(chain) * 4);
    if (!ltc68xx_read_cell_voltages(chain))
        return false;


    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

        for (int j = 0; j < ltc68xx_get_max_cell_count(*device); j++)
            device->openWireTestPuVoltages[j] = ltc68xx_get_cell_voltage(*device, j);
    }

    // Pull-down mode
    for (int i = 0; i < numADOWCommands; i++)
        if (!ltc68xx_cmd_ADOW(chain, chain->adc_mode, false, chain->discharge_mode == DISCHARGE_WHILE_CONVERSION, LTC68XX_ADCV_ALL_CELLS))
            return false;

    delayMillis(ltc68xx_get_adc_time(chain) * 4);
    if (!ltc68xx_read_cell_voltages(chain))
        return false;


    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        for (int j = 0; j < ltc68xx_get_max_cell_count(*device); j++)
            device->openWireTestPdVoltages[j] = ltc68xx_get_cell_voltage(*device, j);
    }

    // Compare voltages
    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        int cellCount = ltc68xx_get_max_cell_count(*device);
        device->error.open_wire_cell_pin[0] = device->openWireTestPuVoltages[0] < 0.001;
        for (int j = 1; j < cellCount; j++)
            device->error.open_wire_cell_pin[j] = device->openWireTestPuVoltages[j + 1] - device->openWireTestPdVoltages[j + 1] < 0.4f; // V
        device->error.open_wire_cell_pin[cellCount] = device->openWireTestPuVoltages[cellCount - 1] < 0.001;
    }

    return ltc68xx_cmd(chain, LTC68XX_CMD_CLRCELL);
}
