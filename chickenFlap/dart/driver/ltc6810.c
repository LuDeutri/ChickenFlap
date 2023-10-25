#include "../../../chickenFlap/dart/driver/ltc6810.h"

uint8_t ltc6810_buffer[128];

#define LTC6810_BUFFER_OFFSET(i) ((LTC6810_BYTES_PER_REGISTER_GROUP + 2) * (i))
#define LTC6810_BUFFER_PEC(i) ((LTC6810_BUFFER_OFFSET(i) + LTC6810_BYTES_PER_REGISTER_GROUP))

bool ltc6810_cmd(ltc6810_chain_t* chain, uint16_t cmd) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);

	uint8_t txData[4];
	txData[0] = cmd >> 8;
	txData[1] = cmd & 0xFF;

	uint16_t pec = pec15_calc(txData, 2);
	txData[2] = pec >> 8;
	txData[3] = pec & 0xFF;

	digitalWrite(chain->cs, LOW);
	if (!spi_begin(chain->bus))
		return false;
	if (!spi_write(chain->bus, txData, sizeof(txData)))
		return false;
	if (!spi_end(chain->bus))
		return false;
	digitalWrite(chain->cs, HIGH);
	return true;
}

bool ltc6810_read(ltc6810_chain_t* chain, uint16_t cmd) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(LTC6810_BUFFER_OFFSET(chain->numICs) <= sizeof(ltc6810_buffer), DART_ERROR_INVALID_VALUE, false);

	uint8_t txData[4];
	txData[0] = cmd >> 8;
	txData[1] = cmd & 0xFF;

	uint16_t pec = pec15_calc(txData, 2);
	txData[2] = pec >> 8;
	txData[3] = pec & 0xFF;

	// Clear buffer before reading
	memset(ltc6810_buffer, 0, sizeof(ltc6810_buffer));

	digitalWrite(chain->cs, LOW);
	if (!spi_begin(chain->bus))
		return false;
	if (!spi_write(chain->bus, txData, sizeof(txData)))
		return false;
	if (!spi_read(chain->bus, ltc6810_buffer, LTC6810_BUFFER_OFFSET(chain->numICs)))
		return false;
	if (!spi_end(chain->bus))
		return false;
	digitalWrite(chain->cs, HIGH);
	return true;
}


bool ltc6810_write(ltc6810_chain_t* chain, uint16_t cmd) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);

	uint8_t txData[4];
	txData[0] = cmd >> 8;
	txData[1] = cmd & 0xFF;

	uint16_t pec = pec15_calc(txData, 2);
	txData[2] = pec >> 8;
	txData[3] = pec & 0xFF;

	digitalWrite(chain->cs, LOW);
	if (!spi_begin(chain->bus))
		return false;
	if (!spi_write(chain->bus, txData, sizeof(txData)))
		return false;
	if (!spi_write(chain->bus, ltc6810_buffer, (LTC6810_BYTES_PER_REGISTER_GROUP + 2) * chain->numICs))
		return false;
	if (!spi_end(chain->bus))
		return false;
	digitalWrite(chain->cs, HIGH);
	return true;
}

bool ltc6810_check_pec(ltc6810_chain_t* chain, ltc6810_t* devices, int index) {
	DART_ASSERT_RETURN(index >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(index < chain->numICs, DART_ERROR_INVALID_VALUE, false);

	ltc6810_t* device = &devices[index];

	// Compare sent PEC with calculated
	uint16_t sentPEC = (ltc6810_buffer[LTC6810_BUFFER_PEC(index)] << 8) | ltc6810_buffer[LTC6810_BUFFER_PEC(index) + 1];
	bool ok = pec15_calc(ltc6810_buffer + LTC6810_BUFFER_OFFSET(index), LTC6810_BYTES_PER_REGISTER_GROUP) == sentPEC;

	if(ok) {
		if (device->pec_error_counter > 0) // prevent underflow
			device->pec_error_counter--;
	}
	else {
		if (device->pec_error_counter < 255) // prevent overflow
			device->pec_error_counter++;
	}

	device->error.pec_error = device->pec_error_counter > 127;
	return ok;
}

bool ltc6810_read_sid(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDSID))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->sid, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->sid));
	}
	return true;
}

bool ltc6810_read_status_a(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDSTATA))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->status_a, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->status_a));
	}
	return true;
}

bool ltc6810_read_status_b(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDSTATB))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->status_b, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->status_b));
	}
	return true;
}

bool ltc6810_read_cellVoltage_a(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDCVA))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(device->cellVoltage_a, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->cellVoltage_a));
	}
	return true;
}

bool ltc6810_read_cellVoltage_b(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDCVB))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->cellVoltage_b, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->cellVoltage_b));
	}
	return true;
}

bool ltc6810_read_aux_a(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDAUXA))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->aux_a, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->aux_a));
	}
	return true;
}

bool ltc6810_read_aux_b(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDAUXB))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->aux_b, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->aux_b));
	}
	return true;
}

bool ltc6810_read_redunantCellVoltage_a(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDSA))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(device->redunantCellVoltage_a, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->redunantCellVoltage_a));
	}
	return true;
}

bool ltc6810_read_redunantCellVoltage_b(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDSB))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(device->redunantCellVoltage_b, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->redunantCellVoltage_b));
	}
	return true;
}

bool ltc6810_read_config(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDCFG))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->config, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->config));
	}
	return true;
}

bool ltc6810_read_comm(ltc6810_chain_t* chain, ltc6810_t* devices) {
	if (!ltc6810_read(chain, LTC6810_CMD_RDCOMM))
		return false;

	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		// Check PEC
		if (!ltc6810_check_pec(chain, devices, i))
			continue;

		memcpy(&device->comm, ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), sizeof(device->comm));
	}
	return true;
}

void ltc6810_write_pec(int index) {
	DART_ASSERT(index >= 0, DART_ERROR_INVALID_VALUE);

	uint16_t pec = pec15_calc(ltc6810_buffer + LTC6810_BUFFER_OFFSET(index), LTC6810_BYTES_PER_REGISTER_GROUP);
	ltc6810_buffer[LTC6810_BUFFER_PEC(index)] = pec >> 8;
	ltc6810_buffer[LTC6810_BUFFER_PEC(index) + 1] = pec >> 8;
}

bool ltc6810_write_config(ltc6810_chain_t* chain, ltc6810_t* devices) {
	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		memcpy(ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), &device->config, sizeof(device->config));
		ltc6810_write_pec(i);
	}
	return ltc6810_write(chain, LTC6810_CMD_WRCFG);
}

bool ltc6810_write_comm(ltc6810_chain_t* chain, ltc6810_t* devices) {
	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		memcpy(ltc6810_buffer + LTC6810_BUFFER_OFFSET(i), &device->comm, sizeof(device->comm));
		ltc6810_write_pec(i);
	}
	return ltc6810_write(chain, LTC6810_CMD_WRCOMM);
}

bool ltc6810_cmd_ADCV(ltc6810_chain_t* chain, ltc6810_adc_mode_t mode, bool dcp, uint8_t ch) {
	uint16_t cmd = LTC6810_CMD_ADCV | (ltc6810_convert_adc_mode(mode) << 7) | (dcp << 4) | ch;
	return ltc6810_cmd(chain, cmd);
}

bool ltc6810_cmd_ADAXD(ltc6810_chain_t* chain, ltc6810_adc_mode_t mode, uint8_t chg) {
	uint16_t cmd = LTC6810_CMD_ADAXD | (ltc6810_convert_adc_mode(mode) << 7) | chg;
	return ltc6810_cmd(chain, cmd);
}

bool ltc6810_cmd_ADSTATD(ltc6810_chain_t* chain, ltc6810_adc_mode_t mode, uint8_t chst) {
	uint16_t cmd = LTC6810_CMD_ADSTATD | (ltc6810_convert_adc_mode(mode) << 7) | chst;
	return ltc6810_cmd(chain, cmd);
}


bool ltc6810_init(ltc6810_chain_t* chain, ltc6810_t* devices) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(devices, DART_ERROR_INVALID_VALUE, false);

	// Setup chain
	chain->ready = false;
	chain->state = LTC6810_STATE_INIT;
	chain->updateCounter = 0;

	// Clear memory of our device structs
	memset(devices, 0, sizeof(ltc6810_t) * chain->numICs);

	if (!ltc6810_wakeup(chain))
		return false;

	// Reset registers of devices
	if (!ltc6810_cmd(chain, LTC6810_CMD_CLRCELL))
		return false;
	if (!ltc6810_cmd(chain, LTC6810_CMD_CLRAUX))
		return false;
	if (!ltc6810_cmd(chain, LTC6810_CMD_CLRSTAT))
		return false;

	// Read sid
	if (!ltc6810_read_sid(chain, devices))
		return false;

	// Setup config for chain
	chain->adc_mode = LTC6810_ADC_7kHz;
	chain->discharge_mode = DISALLOW_DISCHARGE;
	chain->firstStep = true;
	chain->stateTime = millis();

	// Setup config for each device
	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];
		device->config.REFON = true;
		device->config.DTEN = false;
		device->config.ADCOPT = false;
		device->config.MCAL = false;
		device->config.SCONV = false;
		device->config.FDRF = false;
		device->config.DIS_RED = false;

		device->error.thermal_shutdown = false;
	}
	return ltc6810_write_config(chain, devices);
}

bool ltc6810_wakeup(ltc6810_chain_t* chain) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);

	// Produce some communication so ICs wake up
	bool res = true;
	for (uint8_t i = 0; i < 4; i++)
		res &= ltc6810_read(chain, LTC6810_CMD_RDSID);
	return res;
}

uint8_t ltc6810_convert_adc_mode(ltc6810_adc_mode_t mode) {
	switch(mode) {
	case LTC6810_ADC_422Hz:
	case LTC6810_ADC_1kHz:
		return 0;
	case LTC6810_ADC_14kHz:
	case LTC6810_ADC_27kHz:
		return 1;
	default:
	case LTC6810_ADC_3kHz:
	case LTC6810_ADC_7kHz:
		return 2;
	case LTC6810_ADC_26Hz:
	case LTC6810_ADC_2kHz:
		return 3;
	}
}

util_time_t ltc6810_get_adc_time(ltc6810_chain_t* chain) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, 202);

	switch(chain->adc_mode) {
		case LTC6810_ADC_27kHz:
		case LTC6810_ADC_14kHz:
		case LTC6810_ADC_7kHz:
			return 2;
		case LTC6810_ADC_3kHz:
		case LTC6810_ADC_2kHz:
			return 4;
		case LTC6810_ADC_1kHz:
			return 7;
		case LTC6810_ADC_422Hz:
			return 13;
		default:
		case LTC6810_ADC_26Hz:
			return 202;
	}
}

bool ltc6810_has_error(const ltc6810_t device) {
	return device.error.pec_error
		|| device.error.self_test_cell_voltages_fail
		|| device.error.self_test_aux_voltages_fail
		|| device.error.self_test_status_fail
		|| device.error.thermal_shutdown
		|| device.error.ref_voltage_wrong
		|| device.error.analog_power_supply_wrong
		|| device.error.digital_power_supply_wrong
		|| device.error.multiplexer_self_test_fail
		|| device.error.cell_voltage_error
		|| device.error.cell_voltage_any_invalid
		|| device.error.open_wire_error;
}

bool ltc6810_I2C_read16(ltc6810_chain_t* chain, ltc6810_t* devices, uint8_t addr, uint8_t reg, uint16_t* data, size_t dsize){
	//Check input values
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(devices, DART_ERROR_INVALID_VALUE, false);

	if(addr <= 127)
		return false;
	if(!(dsize >= chain->numICs))
		return false;

	for(int i = 0; i < chain->numICs; i++){
		ltc6810_t* device = &devices[i];

		//Write commands for the first transaction
		device->comm[0] = LTC6810_COMM_I2CSTART | (addr << 4) | LTC6810_COMM_I2CNACK;
		device->comm[1] = (reg << 4) | LTC6810_COMM_I2CNACK;
		device->comm[2] = LTC6810_COMM_I2CSTART | ((addr + 1) << 4) | LTC6810_COMM_I2CNACK;
	}
	ltc6810_write_comm(chain, devices);
	ltc6810_write(chain, LTC6810_CMD_STCOMM);
	ltc6810_read_comm(chain, devices);

	//TODO: implement ltc6810_analyze_tx()

	for(int i = 0; i < chain->numICs; i++){
		ltc6810_t* device = &devices[i];

		//Write commands for the second transaction
		device->comm[0] = 0x0ff0;
		device->comm[1] = 0x0ff0 | LTC6810_COMM_I2CNACKSTOP;
		device->comm[2] = 0x0ff0 | LTC6810_COMM_I2CNOP;
	}

	ltc6810_write_comm(chain, devices);
	ltc6810_write(chain, LTC6810_CMD_STCOMM);
	ltc6810_read_comm(chain, devices);

	//TODO: implement ltc6810_analyze_rx()

	//Extract the desired data bits
	for(int i = 0; i < chain->numICs; i++){
		ltc6810_t* device = &devices[i];
		data[i] = ((device->comm[0] & 0x0ff0) << 4) | ((device->comm[1] & 0x0ff0) >> 4);
	}
	return true;
}

float ltc6810_get_die_temperature(const ltc6810_t device) {
	return (device.status_a.ITMP * 0.0001f / 0.0075f) - 273.0f;
}

float ltc6810_get_ref_voltage(const ltc6810_t device) {
	return device.aux_b.REF * 0.0001f;
}

float ltc6810_get_analog_supply_voltage(const ltc6810_t device) {
	return device.status_a.VA * 0.0001f;
}

float ltc6810_get_digital_supply_voltage(const ltc6810_t device) {
	return device.status_b.VD * 0.0001f;
}

float ltc6810_get_cell_voltage(const ltc6810_t device, uint8_t index) {
	DART_ASSERT_RETURN(index < LTC6810_CELL_COUNT, DART_ERROR_INVALID_VALUE, 0);

	if (index >= 3)
		return device.cellVoltage_b[index - 3] * 0.0001f;
	return device.cellVoltage_a[index] * 0.0001f;
}

float ltc6810_get_redundant_cell_voltage(const ltc6810_t device, uint8_t index) {
	DART_ASSERT_RETURN(index < LTC6810_CELL_COUNT, DART_ERROR_INVALID_VALUE, 0);

	if (index >= 3)
		return device.redunantCellVoltage_b[index - 3] * 0.0001f;
	return device.redunantCellVoltage_a[index] * 0.0001f;
}

bool ltc6810_enable_redundant_voltage(ltc6810_chain_t* chain, ltc6810_t* device, bool enable) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_USED(enable);
	DART_NOT_IMPLEMENTED_RETURN(false);
}

bool ltc6810_set_undervoltage(ltc6810_chain_t* chain, ltc6810_t* devices, float voltage) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(devices, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_USED(voltage);
	DART_NOT_IMPLEMENTED_RETURN(false);
}

bool ltc6810_set_overvoltage(ltc6810_chain_t* chain, ltc6810_t* devices, float voltage) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(devices, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_USED(voltage);
	DART_NOT_IMPLEMENTED_RETURN(false);
}

bool ltc6810_set_gpio(ltc6810_chain_t* chain, ltc6810_t* devices, ltc6810_t* device, uint8_t gpio, bool state) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(devices, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(gpio >= 1 && gpio <= 4, DART_ERROR_INVALID_VALUE, false);

	switch(gpio) {
	case 1:
		device->config.GPIO1 = state;
		break;
	case 2:
		device->config.GPIO2 = state;
		break;
	case 3:
		device->config.GPIO3 = state;
		break;
	case 4:
		device->config.GPIO4 = state;
		break;
	}
	return ltc6810_write_config(chain, devices);
}

void ltc6810_set_state(ltc6810_chain_t* chain, ltc6810_driver_state_t newState) {
	DART_NOT_NULL(chain, DART_ERROR_INVALID_VALUE);

	chain->state = newState;
	chain->stateTime = millis();
	chain->firstStep = true;
}

bool ltc6810_update(ltc6810_chain_t* chain, ltc6810_t* devices) {
	DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(devices, DART_ERROR_INVALID_VALUE, false);

	// Read status
	ltc6810_read_status_a(chain, devices);
	ltc6810_read_status_b(chain, devices);

	// Update state machine
	bool firstStep = chain->firstStep;
	chain->firstStep = false;
	chain->updateCounter++;

	// Do ADC state machine
	util_time_t timeInState = (millis() - chain->stateTime);
	switch(chain->state) {
	default:
	case LTC6810_STATE_INIT:
		ltc6810_set_state(chain, LTC6810_STATE_SELF_TEST_MULTIPLEXER);
		break;
	// Self test
	case LTC6810_STATE_SELF_TEST_MULTIPLEXER:
		ltc6810_cmd(chain, LTC6810_CMD_DIAGN);
		ltc6810_set_state(chain, LTC6810_STATE_MEASURE_CELL_VOLTAGES);
		break;

	// Read cell voltages
	case LTC6810_STATE_MEASURE_CELL_VOLTAGES:
		if (firstStep) {
			if (!ltc6810_cmd_ADCV(chain, chain->adc_mode, chain->discharge_mode == DISCHARGE_WHILE_CONVERSION, LTC6810_ADCV_ALL_CELLS))
				return false;
		}
		else if (timeInState > ltc6810_get_adc_time(chain)) {
			if (!ltc6810_read_cellVoltage_a(chain, devices))
				return false;
			if (!ltc6810_read_cellVoltage_b(chain, devices))
				return false;
			ltc6810_set_state(chain, LTC6810_STATE_MEASURE_AUX_VOLTAGES);
		}
		break;
	// Read aux voltages
	case LTC6810_STATE_MEASURE_AUX_VOLTAGES:
		if (firstStep) {
			if (!ltc6810_cmd_ADAXD(chain, chain->adc_mode, 0))
				return false;
		}
		else if (timeInState > ltc6810_get_adc_time(chain)) {
			if (!ltc6810_read_aux_a(chain, devices))
				return false;
			if (!ltc6810_read_aux_b(chain, devices))
				return false;
			ltc6810_set_state(chain, LTC6810_STATE_MEASURE_STATUS_VOLTAGES);
		}
		break;
	// Read status voltages
	case LTC6810_STATE_MEASURE_STATUS_VOLTAGES:
		if (firstStep) {
			if (!ltc6810_cmd_ADSTATD(chain, chain->adc_mode, 0))
				return false;
		}
		else if (timeInState > ltc6810_get_adc_time(chain)) {
			chain->ready = true;
			ltc6810_set_state(chain, LTC6810_STATE_MEASURE_CELL_VOLTAGES);
		}
		break;
	}

	// Check for any errors
	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];

		float refVoltage = ltc6810_get_ref_voltage(*device);
		device->error.ref_voltage_wrong = refVoltage < 2.99f || refVoltage > 3.01f;

		float analogVoltage = ltc6810_get_analog_supply_voltage(*device);
		device->error.analog_power_supply_wrong = analogVoltage < 4.5f || analogVoltage > 5.5f;

		float digitalVoltage = ltc6810_get_digital_supply_voltage(*device);
		device->error.digital_power_supply_wrong = digitalVoltage < 2.7f || digitalVoltage > 3.6f;

		// Check cell voltages
		device->error.cell_voltage_error = device->status_b.C1OV || device->status_b.C1UV ||
				device->status_b.C2OV || device->status_b.C2UV ||
				device->status_b.C3OV || device->status_b.C3UV ||
				device->status_b.C4OV || device->status_b.C4UV ||
				device->status_b.C5OV || device->status_b.C5UV ||
				device->status_b.C6OV || device->status_b.C6UV;

		// Check cell voltages range
		for (int j = 0; j < 3; j++)
			device->error.cell_voltage_invalid[j] = device->cellVoltage_a[j] > LTC6810_ADC_MAX_VALUE;
		for (int j = 0; j < 3; j++)
			device->error.cell_voltage_invalid[3 + j] = device->cellVoltage_b[j] > LTC6810_ADC_MAX_VALUE;

		device->error.cell_voltage_any_invalid = false;
		for (int j = 0; j < LTC6810_CELL_COUNT; j++)
			device->error.cell_voltage_any_invalid |= device->error.cell_voltage_invalid[j];


		// Latch thermal shutdown (register on the IC is reset by read command above)
		if (chain->ready)
			device->error.thermal_shutdown |= device->status_b.THSD;
		else
			device->error.thermal_shutdown = device->status_b.THSD;
		device->error.multiplexer_self_test_fail = device->status_b.MUXFAIL;
	}

	return true;
}

void ltc6810_dump(ltc6810_chain_t* chain, ltc6810_t* devices, int uart) {
	DART_NOT_NULL(chain, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(devices, DART_ERROR_INVALID_VALUE);

	dartUART_writeLine(uart, "");
	dartUART_formatLine(uart, "chain = bus: %d, numICs: %d, ready: %d, state: %d", chain->bus, chain->numICs, chain->ready, chain->state);
	for (int i = 0; i < chain->numICs; i++) {
		ltc6810_t* device = &devices[i];
		dartUART_formatLine(uart, "device[%d] = sid: 0x%x%x, rev: %d", i, (int)(device->sid >> 32), (int)(device->sid & 0xFFFFFFFF), (int)device->status_b.REV);

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
		dartUART_formatLine(uart, "multiplexer_self_test_fail: %d, cell_voltage_error: %d",
				device->error.multiplexer_self_test_fail,
				device->error.cell_voltage_error);
		dartUART_formatLine(uart, "open_wire_error: %d",
				device->error.open_wire_error);

		dartUART_formatLine(uart, "ltc6810_get_die_temperature(): %d C",
				(int)ltc6810_get_die_temperature(*device));
		dartUART_formatLine(uart, "ltc6810_get_ref_voltage(): %d mV",
				(int)(ltc6810_get_ref_voltage(*device) * 1000));
		dartUART_formatLine(uart, "ltc6810_get_analog_supply_voltage(): %d mV",
				(int)(ltc6810_get_analog_supply_voltage(*device) * 1000));
		dartUART_formatLine(uart, "ltc6810_get_digital_supply_voltage(): %d mV",
				(int)(ltc6810_get_digital_supply_voltage(*device) * 1000));

		for (int j = 0; j < LTC6810_CELL_COUNT; j++) {
			dartUART_formatLine(uart, "ltc6810_get_cell_voltage(%d): %d mV", j,
					(int)(ltc6810_get_cell_voltage(*device, j) * 1000));
		}
		dartUART_writeLine(uart, "");
	}
	dartUART_writeLine(uart, "");
}
