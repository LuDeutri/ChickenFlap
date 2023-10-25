#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_data.h"

bool ltc68xx_has_error(const ltc68xx_t device) {

	return device.error.pec_error
        || device.error.self_test_config_read_back_fail
        || device.error.self_test_multiplexer_fail
        || device.error.self_test_cell_voltages_fail
        || device.error.self_test_aux_voltages_fail
        || device.error.self_test_status_fail
        || device.error.self_test_open_wire_fail
        || device.error.thermal_shutdown
        || device.error.ref_voltage_wrong
        || device.error.analog_power_supply_wrong
        || device.error.digital_power_supply_wrong
        || (device.error.cell_voltage_error && !device.ignoreCellVoltageError)
        || device.error.cell_voltage_any_invalid;
}

uint64_t ltc68xx_get_sid(const ltc68xx_t device) {
    return from_intel_uint64(device.sid);
}

float ltc68xx_get_die_temperature(const ltc68xx_t device) {
    return (from_intel_uint16(device.status_a.ITMP) * 0.0001f / 0.0075f) - 273.0f;
}

float ltc68xx_get_ref_voltage(const ltc68xx_t device) {
    return from_intel_uint16(device.aux_b.REF) * 0.0001f;
}

float ltc68xx_get_analog_supply_voltage(const ltc68xx_t device) {
    return from_intel_uint16(device.status_a.VA) * 0.0001f;
}

float ltc68xx_get_digital_supply_voltage(const ltc68xx_t device) {
    return from_intel_uint16(device.status_b.VD) * 0.0001f;
}

uint16_t ltc68xx_get_raw_cell_voltage(const ltc68xx_t device, uint8_t index) {
    DART_ASSERT_RETURN(index < LTC68XX_MAX_CELL_COUNT, DART_ERROR_INVALID_VALUE, 0);
    DART_ASSERT_RETURN(index < ltc68xx_get_max_cell_count(device), DART_ERROR_INVALID_VALUE, 0);

    if (index >= 15)
        return from_intel_uint16(device.cellVoltage_f[index - 15]);
    else if (index >= 12)
        return from_intel_uint16(device.cellVoltage_e[index - 12]);
    else if (index >= 9)
        return from_intel_uint16(device.cellVoltage_d[index - 9]);
    else if (index >= 6)
        return from_intel_uint16(device.cellVoltage_c[index - 6]);
    else if (index >= 3)
        return from_intel_uint16(device.cellVoltage_b[index - 3]);
    else
        return from_intel_uint16(device.cellVoltage_a[index]);
}

float ltc68xx_get_cell_voltage(const ltc68xx_t device, uint8_t index) {
    return ltc68xx_get_raw_cell_voltage(device, index) * 0.0001f;
}

bool ltc68xx_set_undervoltage(ltc68xx_chain_t* chain, float voltage) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_USED(voltage);
    DART_NOT_IMPLEMENTED_RETURN(false);
}

bool ltc68xx_set_overvoltage(ltc68xx_chain_t* chain, float voltage) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_USED(voltage);
    DART_NOT_IMPLEMENTED_RETURN(false);
}

bool ltc68xx_set_discharge(ltc68xx_chain_t* chain, int deviceIndex, uint8_t cell, bool state, bool writeConfigToChain) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(deviceIndex >= 0 && deviceIndex < chain->numICs, DART_ERROR_INVALID_VALUE, false);

    ltc68xx_t* device = &chain->devices[deviceIndex];
    DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(cell < ltc68xx_get_max_cell_count(*device), DART_ERROR_INVALID_VALUE, false);

    if (cell == 17)
        device->config_b.DCC18 = state;
    else if (cell == 16)
        device->config_b.DCC17 = state;
    else if (cell >= 12) {
        // DCC16 is left bit, DCC13 is right bit
        int index = (cell - 12);
        if (state)
            device->config_b.DCC16_to_13 |= (1 << index);
        else
            device->config_b.DCC16_to_13 &= ~(1 << index);
    }
    else if (cell >= 8) {
        // DCC12 is left bit, DCC9 is right bit
        int index = (cell - 8);
        if (state)
            device->config_a.DCC12_to_9 |= (1 << index);
        else
            device->config_a.DCC12_to_9 &= ~(1 << index);
    }
    else {
        // DCC8 is left bit, DCC1 is right bit
        if (state)
            device->config_a.DCC8_to_1 |= (1 << cell);
        else
            device->config_a.DCC8_to_1 &= ~(1 << cell);
    }

    if (writeConfigToChain)
        return ltc68xx_write_config(chain);
    return true;
}

bool ltc68xx_set_gpio_pulldown(ltc68xx_chain_t* chain, int deviceIndex, uint8_t gpio, bool state, bool writeConfigToChain) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(deviceIndex >= 0 && deviceIndex < chain->numICs, DART_ERROR_INVALID_VALUE, false);
    // Find and check device
    ltc68xx_t* device = &chain->devices[deviceIndex];
    DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(gpio <= ltc68xx_get_gpio_count(*device), DART_ERROR_INVALID_VALUE, false);

    // Update GPIO flag (pull-down off = 1)
    if (gpio >= 5) {
        int index = gpio - 5;
        // GPIO5_to_1 goes from GPIO9 (left bit) to GPIO6 (right bit)
        if (!state)
            device->config_b.GPIO9_to_6 |= (1 << index);
        else
            device->config_b.GPIO9_to_6 &= ~(1 << index);
    }
    else {
        // GPIO5_to_1 goes from GPIO5 (left bit) to GPIO1 (right bit)
        if (!state)
            device->config_a.GPIO5_to_1 |= (1 << gpio);
        else
            device->config_a.GPIO5_to_1 &= ~(1 << gpio);
    }

    if (writeConfigToChain)
        return ltc68xx_write_config(chain);
    return true;
}

bool ltc68xx_get_gpio(const ltc68xx_t device, uint8_t gpio) {
    DART_ASSERT_RETURN(gpio <= ltc68xx_get_gpio_count(device), DART_ERROR_INVALID_VALUE, false);

    // Check if pin is set
    if (gpio >= 5)
        return (device.config_b_rx.GPIO9_to_6 & (1 << (gpio - 5))) != 0;
    return (device.config_a_rx.GPIO5_to_1 & (1 << gpio)) != 0;
}

float ltc68xx_get_gpio_voltage(const ltc68xx_t device, uint8_t gpio) {
   DART_ASSERT_RETURN(gpio <= ltc68xx_get_gpio_count(device), DART_ERROR_INVALID_VALUE, false);

   // GPIO are offset by 1
   if (device.type == LTC6810)
       gpio += 1;

   uint16_t raw = 0;
   switch(gpio) {
   case 0:
        raw = device.aux_a.G1V; // This is S0V on LTC6810 devices, so G2V is actually G1V
        break;
   case 1:
        raw = device.aux_a.G2V; // This is G1V on LTC6810 devices, and so on...
        break;
   case 2:
        raw = device.aux_a.G3V;
        break;
   case 3:
        raw = device.aux_b.G4V;
        break;
   case 4:
        raw = device.aux_b.G5V;
        break;
   default:
       DART_NOT_IMPLEMENTED_RETURN(0.0f);
   }
   return from_intel_uint16(raw) * 0.0001f;
}
