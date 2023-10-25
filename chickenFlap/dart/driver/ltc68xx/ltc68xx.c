#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx.h"

bool ltc68xx_init(ltc68xx_chain_t* chain) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(chain->numICs > 0, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);

	// Setup chain
	chain->ready = false;
	chain->state = LTC68XX_STATE_INIT;
	chain->updateCounter = 0;

	// Check which features we can use
	chain->useADAXD = false;
	chain->useADSTATD = false;
	chain->canUseSID = true;
	for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

        if (device->type == LTC6813)
            chain->needLTC6813Registers = true;
        if (device->type != LTC6810)
            chain->canUseSID = false;
	}

	// Wake up chain
	util_timer_t wakeupTimer = createTimerNoOffset(chain->wakeupTime);
	while(!hasTimerElapsed(&wakeupTimer))
        ltc68xx_wakeup(chain);

    // Read SID
    if (chain->canUseSID && !ltc68xx_read_sid(chain))
    	return false;

	// Reset registers of devices
    if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRCELL))
		return false;
	if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRAUX))
		return false;
	if (!ltc68xx_cmd(chain, LTC68XX_CMD_CLRSTAT))
		return false;

	// Setup configuration for chain
	chain->adc_mode = LTC68XX_ADC_7kHz;
	chain->discharge_mode = DISALLOW_DISCHARGE;
	chain->firstStep = true;
	chain->stateTime = millis();

	// Setup configuration for each device
	for (int i = 0; i < chain->numICs; i++) {
		ltc68xx_t* device = &chain->devices[i];

		// Default configuration
		device->config_a.GPIO5_to_1 = 0x1F; // disable every pull-down
		device->config_a.REFON = true;
		device->config_a.DTEN = false;
		device->config_a.ADCOPT = false;
		device->config_a.DCC8_to_1 = 0;
		device->config_a.DCTO = 0;
		device->config_a.DCC12_to_9 = 0;
		device->config_a.VUV = 0;
		device->config_a.VOV = 4095;

		// Clear error flag
		memset(&device->error, false, sizeof(device->error));
	}

    // Do configuration write/read check
	if (!ltc68xx_write_config(chain))
	    return false;
	if (!ltc68xx_read_config(chain))
	    return false;

	for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];

        device->error.self_test_config_read_back_fail = (device->config_a.DCC12_to_9 != device->config_a_rx.DCC12_to_9)
                || (device->config_a.DCC8_to_1 != device->config_a_rx.DCC8_to_1)
                || (device->config_a.VUV != device->config_a_rx.VUV)
                || (device->config_a.VOV != device->config_a_rx.VOV);
	}
	return true;
}

bool ltc68xx_is_ready(ltc68xx_chain_t* chain) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    return chain->ready;
}

void ltc68xx_set_state(ltc68xx_chain_t* chain, ltc68xx_driver_state_t newState) {
	DART_NOT_NULL(chain, DART_ERROR_INVALID_VALUE);

	chain->state = newState;
	chain->stateTime = millis();
	chain->firstStep = true;
}

bool ltc68xx_update(ltc68xx_chain_t* chain) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(chain->numICs > 0, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);

	// Read status and configuration (will also wake up chain)
	ltc68xx_read_status(chain);
	ltc68xx_read_config(chain);

	// Update state machine
	bool firstStep = chain->firstStep;
	chain->firstStep = false;
	chain->updateCounter++;

	// Do ADC state machine
	util_time_t timeInState = (millis() - chain->stateTime);
	switch(chain->state) {
	default:
	case LTC68XX_STATE_INIT:
	    if (chain->skipSelfTest)
	        ltc68xx_set_state(chain, LTC68XX_STATE_MEASURE_AUX_VOLTAGES);
	    else
		    ltc68xx_set_state(chain, LTC68XX_STATE_SELF_TEST_MULTIPLEXER);
		break;
	// Self test
	case LTC68XX_STATE_SELF_TEST_MULTIPLEXER:
		if (firstStep)
		    ltc68xx_cmd(chain, LTC68XX_CMD_DIAGN);
		else if (timeInState > 4) { // we need to wait 4ms until the multiplexer test is done
		    ltc68xx_set_state(chain, LTC68XX_STATE_SELF_TEST_CELL_VOLTAGES);
		    for (int i = 0; i < chain->numICs; i++) {
                ltc68xx_t* device = &chain->devices[i];
                DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
                device->error.self_test_multiplexer_fail = device->status_b.MUXFAIL;
            }
		}
		break;
	case LTC68XX_STATE_SELF_TEST_CELL_VOLTAGES:
	    // Perform the two self test modes (1 and 2)
	    if (!ltc68xx_self_test_cell_voltages(chain, 1) || !ltc68xx_self_test_cell_voltages(chain, 2)) {
	        // Self test itself was not successful for whole chain
	        for (int i = 0; i < chain->numICs; i++) {
	            ltc68xx_t* device = &chain->devices[i];
                DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	            device->error.self_test_cell_voltages_fail = true;
	        }
	    }
	    ltc68xx_set_state(chain, LTC68XX_STATE_SELF_TEST_AUX_VOLTAGES);
	    break;
	case LTC68XX_STATE_SELF_TEST_AUX_VOLTAGES:
	    // Perform the two self test modes (1 and 2)
        if (!ltc68xx_self_test_aux_voltages(chain, 1) || !ltc68xx_self_test_aux_voltages(chain, 2)) {
            // Self test itself was not successful for whole chain
            for (int i = 0; i < chain->numICs; i++) {
                ltc68xx_t* device = &chain->devices[i];
                DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
                device->error.self_test_aux_voltages_fail = true;
            }
        }
	    ltc68xx_set_state(chain, LTC68XX_STATE_SELF_TEST_STATUS_VOLTAGES);
	    break;
	case LTC68XX_STATE_SELF_TEST_STATUS_VOLTAGES:
	    // Perform the two self test modes (1 and 2)
        if (!ltc68xx_self_test_status_voltages(chain, 1) || !ltc68xx_self_test_status_voltages(chain, 2)) {
            // Self test itself was not successful for whole chain
            for (int i = 0; i < chain->numICs; i++) {
                ltc68xx_t* device = &chain->devices[i];
                DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
                device->error.self_test_status_fail = true;
            }
        }
        ltc68xx_set_state(chain, LTC68XX_STATE_SELF_TEST_OPEN_WIRE);
	    break;
	case LTC68XX_STATE_SELF_TEST_OPEN_WIRE:
	    if (!ltc68xx_self_test_open_wire(chain)) {
            // Self test itself was not successful for whole chain
            for (int i = 0; i < chain->numICs; i++) {
                ltc68xx_t* device = &chain->devices[i];
                DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
                device->error.self_test_open_wire_fail = true;
            }
        }
	    // Measure aux/status voltages first, to check for any errors directly
	    ltc68xx_set_state(chain, LTC68XX_STATE_MEASURE_AUX_VOLTAGES);
	    break;
	// Read aux voltages
	case LTC68XX_STATE_MEASURE_AUX_VOLTAGES:
		if (firstStep) {
		    if (chain->useADAXD && !ltc68xx_cmd_ADAXD(chain, chain->adc_mode, 0))
                return false;
            if (!chain->useADAXD && !ltc68xx_cmd_ADAX(chain, chain->adc_mode, 0))
                return false;
		}
		else if (timeInState > ltc68xx_get_adc_time(chain)) {
			if (!ltc68xx_read_aux(chain))
				return false;
			ltc68xx_set_state(chain, LTC68XX_STATE_MEASURE_STATUS_VOLTAGES);
		}
		break;
	// Read status voltages
	case LTC68XX_STATE_MEASURE_STATUS_VOLTAGES:
		if (firstStep) {
            if (chain->useADSTATD && !ltc68xx_cmd_ADSTATD(chain, chain->adc_mode, 0))
                return false;
			if (!chain->useADSTATD && !ltc68xx_cmd_ADSTAT(chain, chain->adc_mode, 0))
				return false;
		}
		else if (timeInState > ltc68xx_get_adc_time(chain)) {
			ltc68xx_set_state(chain, LTC68XX_STATE_MEASURE_CELL_VOLTAGES);
		}
		break;
    // Read cell voltages
    case LTC68XX_STATE_MEASURE_CELL_VOLTAGES:
        if (firstStep) {
            if (!ltc68xx_cmd_ADCV(chain, chain->adc_mode, chain->discharge_mode == DISCHARGE_WHILE_CONVERSION, LTC68XX_ADCV_ALL_CELLS))
                return false;
        }
        else if (timeInState > ltc68xx_get_adc_time(chain)) {
            ltc68xx_read_cell_voltages(chain);

            chain->ready = true;

            // Only fetch aux/status voltages every other update step
            if (chain->updateCounter % 5 == 0)
                ltc68xx_set_state(chain, LTC68XX_STATE_MEASURE_AUX_VOLTAGES);
            else
                ltc68xx_set_state(chain, LTC68XX_STATE_MEASURE_CELL_VOLTAGES);
        }
        break;
	}

	// Check for any errors
	if (chain->ready) {
        for (int i = 0; i < chain->numICs; i++) {
            ltc68xx_t* device = &chain->devices[i];
            DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

            float refVoltage = ltc68xx_get_ref_voltage(*device);
            device->error.ref_voltage_wrong = refVoltage < 2.99f || refVoltage > 3.01f;

            float analogVoltage = ltc68xx_get_analog_supply_voltage(*device);
            device->error.analog_power_supply_wrong = analogVoltage < 4.5f || analogVoltage > 5.5f;

            float digitalVoltage = ltc68xx_get_digital_supply_voltage(*device);
            device->error.digital_power_supply_wrong = digitalVoltage < 2.7f || digitalVoltage > 3.6f;

            // Check cell voltages
            device->error.cell_voltage_error = device->status_b.C1OV || device->status_b.C1UV ||
                    device->status_b.C2OV || device->status_b.C2UV ||
                    device->status_b.C3OV || device->status_b.C3UV ||
                    device->status_b.C4OV || device->status_b.C4UV ||
                    device->status_b.C5OV || device->status_b.C5UV ||
                    device->status_b.C6OV || device->status_b.C6UV;

            if (ltc68xx_get_max_cell_count(*device) > 6) {
                device->error.cell_voltage_error |= device->status_b.C7OV || device->status_b.C7UV;
                device->error.cell_voltage_error |= device->status_b.C8OV || device->status_b.C8UV;
                device->error.cell_voltage_error |= device->status_b.C9OV || device->status_b.C9UV;
                device->error.cell_voltage_error |= device->status_b.C10OV || device->status_b.C10UV;
                device->error.cell_voltage_error |= device->status_b.C11OV || device->status_b.C11UV;
                device->error.cell_voltage_error |= device->status_b.C12OV || device->status_b.C12UV;
            }

            // Check cell voltages range
            device->error.cell_voltage_any_invalid = false;
            for (int j = 0; j < ltc68xx_get_max_cell_count(*device); j++) {
                bool invalid =  ltc68xx_get_raw_cell_voltage(*device, j) > LTC68XX_ADC_MAX_VALUE;
                device->error.cell_voltage_invalid[j] = invalid;
                device->error.cell_voltage_any_invalid |= invalid;
            }


            // Latch thermal shutdown (register on the IC is reset by read command above)
            if (chain->ready)
                device->error.thermal_shutdown |= device->status_b.THSD;
            else
                device->error.thermal_shutdown = device->status_b.THSD;
        }
	}

	return true;
}

void ltc68xx_dump(ltc68xx_chain_t* chain, int uart) {
	DART_NOT_NULL(chain, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(chain->numICs > 0, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(chain->devices, DART_ERROR_INVALID_VALUE);

	dartUART_writeLine(uart, "");
	dartUART_formatLine(uart, "chain = bus: %d, numICs: %d, ready: %d, state: %d", chain->bus, chain->numICs, chain->ready, chain->state);
	for (int i = 0; i < chain->numICs; i++) {
		ltc68xx_t* device = &chain->devices[i];
		DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);

		dartUART_formatLine(uart, "device[%d] = type: %d, sid: 0x%x%x, rev: %d", i, device->type, (int)(ltc68xx_get_sid(*device) >> 32), (int)(ltc68xx_get_sid(*device) & 0xFFFFFFFF), (int)device->status_b.REV);

		dartUART_formatLine(uart, "pec_error_counter: %d, self_test_cell_voltages_fail: %d",
				device->pec_error_counter,
				device->error.self_test_cell_voltages_fail);
		dartUART_formatLine(uart, "self_test_aux_voltages_fail: %d, self_test_status_fail: %d",
				device->error.self_test_aux_voltages_fail,
				device->error.self_test_status_fail);
		dartUART_formatLine(uart, "thermal_shutdown: %d, ref_voltage_wrong: %d",
				device->error.thermal_shutdown,
				device->error.ref_voltage_wrong);
		dartUART_formatLine(uart, "analog_power_supply_wrong: %d, digital_power_supply_wrong: %d",
				device->error.analog_power_supply_wrong,
				device->error.digital_power_supply_wrong);
		dartUART_formatLine(uart, "self_test_multiplexer_fail: %d, cell_voltage_error: %d",
				device->error.self_test_multiplexer_fail,
				device->error.cell_voltage_error);
		dartUART_formatLine(uart, "self_test_open_wire_fail: %d, self_test_config_read_back_fail",
				device->error.self_test_open_wire_fail, device->error.self_test_config_read_back_fail);

		dartUART_formatLine(uart, "ltc68xx_get_die_temperature(): %d C",
				(int)ltc68xx_get_die_temperature(*device));
		dartUART_formatLine(uart, "ltc68xx_get_ref_voltage(): %d mV",
				(int)(ltc68xx_get_ref_voltage(*device) * 1000));
		dartUART_formatLine(uart, "ltc68xx_get_analog_supply_voltage(): %d mV",
				(int)(ltc68xx_get_analog_supply_voltage(*device) * 1000));
		dartUART_formatLine(uart, "ltc68xx_get_digital_supply_voltage(): %d mV", (int)(ltc68xx_get_digital_supply_voltage(*device) * 1000));

		for (int j = 0; j < ltc68xx_get_gpio_count(*device); j++) {
            dartUART_formatLine(uart, "ltc68xx_get_gpio(%d): %d", j, ltc68xx_get_gpio(*device, j));
        }
		for (int j = 0; j < ltc68xx_get_max_cell_count(*device); j++) {
			dartUART_formatLine(uart, "ltc68xx_get_cell_voltage(%d): %d mV", j, (int)(ltc68xx_get_cell_voltage(*device, j) * 1000));
		}
		for (int j = 0; j < ltc68xx_get_max_cell_count(*device) + 1; j++) {
            dartUART_formatLine(uart, "open_wire(C%d): %d", j, device->error.open_wire_cell_pin[j]);
        }
		dartUART_writeLine(uart, "");
	}
	dartUART_writeLine(uart, "");
}
