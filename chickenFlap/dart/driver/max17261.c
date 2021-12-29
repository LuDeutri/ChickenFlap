#include "../../../chickenFlap/dart/driver/max17261.h"

#include "../../../chickenFlap/dart/util/motorola.h"

bool max17261_init(max17261_device_t *device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

// Step 0: Check for POR
	if ((max17261_readRegister(device, MAX17261_REG_STATUS) & 0x0002) != 0) {
// Step 1: Delay until FSTAT.DNR bit == 0
		while (max17261_readRegister(device, MAX17261_REG_FSTAT) & 0x01) { // wait for the MAX1726x to complete its startup operations
			delayMillis(10);
		}
// Step 2: Initialize Configuration
		uint16_t hibCFG = max17261_readRegister(device, 0xBA); // Store original HibCFG value

		if (!max17261_write16(device, 0x60, 0x90))
			return false;						// Exit Hibernate Mode step 1
		if (!max17261_write16(device, 0xBA, 0x00))
			return false;						// Exit Hibernate Mode step 2
		if (!max17261_write16(device, 0x60, 0x00))
			return false;						// Exit Hibernate Mode step 3
// Step 2.1: OPTION 1 EZ Config
		if (!max17261_write16(device, MAX17261_REG_DESIGN_CAP, 0x1D4C))
			return false;	// Write DesignCap
		if (!max17261_write16(device, MAX17261_REG_I_CHG_TERM, 0x333))
			return false;		// Write IchgTerm
		if (!max17261_write16(device, MAX17261_REG_V_EMPTY, 0xA561))
			return false;		// Write VEmpty

		if (device->chargeVoltage > 4275) {
			if (!max17261_write16(device, MAX17261_REG_MODEL_CFG, 0x8400))
				return false;	// Write ModelCFG
		} else {
			if (!max17261_write16(device, MAX17261_REG_MODEL_CFG, 0x8000))
				return false;	// Write ModelCFG
		}

		while (max17261_readRegister(device, MAX17261_REG_MODEL_CFG) & 0x8000) { // wait until the MSB of ModelCFG == 0
			delayMillis(10);
		}
		if (!max17261_write16(device, 0xBA, hibCFG))
			return false; 						// Restore Original HibCFG value
// Step: 3 Initialization Complete
		uint16_t status = max17261_readRegister(device, MAX17261_REG_STATUS);
		writeAndVerifyRegister(device, MAX17261_REG_STATUS, status & 0xFFFD); //Write and Verify Status with POR bit cleared
	} else {
// Step 3.1: If the host recognizes the battery pack as one with a saved history restore all the saved parameters otherwise, continue to Step 3.2
		if (device->learned.fullCapNom != 0) {
			max17261_restoreLearnedParameters(device); // Go to Step 3.6
		} else {
			max17261_update(device); // Go to Step 3.2
		}
	}
	return true;

	uint16_t config2 = 0;
	i2c_read_register(device->bus, device->adress, MAX17261_REG_CONFIG2,
			(uint8_t*) &config2, sizeof(config2));
	/*
	 if(!max17261_wakeup(device))
	 return false;
	 if(!max17261_write16(device, MAX17261_REG_DESIGN_CAP, device->regs.repCap))
	 return false;
	 if(!max17261_write16(device, MAX17261_REG_CONFIG, device->regs.config))
	 return false;
	 if(!max17261_write16(device, MAX17261_REG_CONFIG2, device->regs.config2))
	 return false;
	 */
}

void max17261_update(max17261_device_t *device) {
	DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);

// Step 3.2: Check for MAX1726x Reset
	max17261_checkReset(device);
	/*
	 Step 3.3: Read the RepCap and RepSOC Registers
	 device->regs.repCap = max17261_readRegister(device, MAX17261_REG_REP_CAP) / MAX17261_SCALE_CAPACITY;	// Read RepCap - mAh
	 device->regs.repSOC = max17261_readRegister(device, MAX17261_REG_REP_SOC);								// Read RepSOC - %
	 Step 3.4: Read the Remaining TTE Register
	 device->regs.tte = max17261_readRegister(device, MAX17261_REG_TTE) / MAX17261_SCALE_TIME; 				// Read tte - s
	 */
// Step 3.5: Save Learned Parameters
	device->learned.rComp0 = max17261_readRegister(device,
			MAX17261_REG_R_COMP0);
	device->learned.tempCo = max17261_readRegister(device,
			MAX17261_REG_TEMP_CO);
	device->learned.fullCapRep = max17261_readRegister(device,
			MAX17261_REG_FULL_CAP_REP);
	device->learned.cycles = max17261_readRegister(device, MAX17261_REG_CYCLES);
	device->learned.fullCapNom = max17261_readRegister(device,
			MAX17261_REG_FULL_CAP_NOM);
}

// Step 3.6: Restoring Learned Parameters
void max17261_restoreLearnedParameters(max17261_device_t *device) {
	DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);

	writeAndVerifyRegister(device, MAX17261_REG_R_COMP0, device->learned.rComp0);
	writeAndVerifyRegister(device, MAX17261_REG_TEMP_CO, device->learned.tempCo);
	writeAndVerifyRegister(device, MAX17261_REG_FULL_CAP_REP, device->learned.fullCapRep);
	writeAndVerifyRegister(device, MAX17261_REG_CYCLES, device->learned.cycles);
	writeAndVerifyRegister(device, MAX17261_REG_FULL_CAP_NOM, device->learned.fullCapNom);
}

void max17261_checkReset(max17261_device_t *device) {
	DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);

	if ((max17261_readRegister(device, MAX17261_REG_STATUS) & 0x0002) != 0) // Read POR bit in Status Register
		max17261_init(device);												// init new
}

void writeAndVerifyRegister(max17261_device_t *device, uint8_t regAdr, uint16_t data) {
	DART_NOT_NULL(device, DART_ERROR_INVALID_VALUE);

	uint16_t readingValue;
	int attempt = 0;
	do {
		max17261_write16(device, regAdr, data);
		delayMillis(1);
		readingValue = max17261_readRegister(device, MAX17261_REG_STATUS);

	} while (data != readingValue && attempt++ < 3);
}

bool max17261_write16(max17261_device_t *device, uint8_t regAdr, uint16_t data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	data = to_motorola_uint16(data);
	return i2c_write_register(device->bus, device->adress, regAdr, (uint8_t*) &data, sizeof(data));
}

bool max17261_read(max17261_device_t *device, uint8_t regAdr, uint8_t *data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	if (!i2c_read_register(device->bus, device->adress, regAdr, data, sizeof(data)))
		return false;

	return true;
}

uint16_t max17261_readRegister(max17261_device_t *device, uint8_t regAdr) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint16_t data = 0;
	if (!i2c_read_register(device->bus, device->adress, regAdr, (uint8_t*) &data, sizeof(data)))
		return 256;
	data = from_motorola_uint16(data);
	return data;
}

bool max17261_read_serialID(max17261_device_t *device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	//test
	uint16_t configtest1;
	i2c_read_register(device->bus, device->adress, 0xB4, (uint8_t*) &configtest1, sizeof(configtest1));

	//set
	//uint16_t cmd = cmd & 1 << 4; //to_motorola_uint16(1 << 4);
	uint16_t cmd = 001 << AT_RATE_EN; 	// set AtRateEn Bit 1
	cmd = cmd | 1 << DPEN;				// set DPEN Bit 1
	cmd = ~cmd;							// invert cmd

	uint16_t config2 = 0;
	i2c_read_register(device->bus, device->adress, MAX17261_REG_CONFIG2,
			(uint8_t*) &config2, sizeof(config2));

	config2 = config2 & cmd;			// combine current config2 with cmd

	if (!i2c_write_register(device->bus, device->adress, MAX17261_REG_CONFIG2,
			(uint8_t*) &cmd, sizeof(cmd)))
		return false;

	uint16_t configtest2;
	i2c_read_register(device->bus, device->adress, 0xB4, (uint8_t*) &configtest2, sizeof(configtest2));

	uint16_t lol = 0;
	i2c_read_register(device->bus, device->adress, MAX17261_SERIAL_NUMBER_WORD_0, (uint8_t*) &lol, sizeof(lol));

	uint8_t sidReg[] = {
	MAX17261_SERIAL_NUMBER_WORD_0,
	MAX17261_SERIAL_NUMBER_WORD_1,
	MAX17261_SERIAL_NUMBER_WORD_2,
	MAX17261_SERIAL_NUMBER_WORD_3,
	MAX17261_SERIAL_NUMBER_WORD_4,
	MAX17261_SERIAL_NUMBER_WORD_5,
	MAX17261_SERIAL_NUMBER_WORD_6,
	MAX17261_SERIAL_NUMBER_WORD_7 };

	for (int i = 0; i < 8; i++) {
		if (!i2c_read_register(device->bus, device->adress, sidReg[i], (uint8_t*) &device->sid[i], sizeof(uint16_t)))
			return false;
	}
	return true;
}

bool max17261_read_capacity(max17261_device_t *device, uint8_t *capacityArr,
		size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size >= 4, DART_ERROR_INVALID_VALUE, false);

	return i2c_read_register(device->bus, device->adress, MAX17261_REG_REP_CAP, capacityArr, sizeof(capacityArr));
}

bool max17261_read_voltage(max17261_device_t *device, uint8_t *value,
		size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size >= 2, DART_ERROR_INVALID_VALUE, false);

	return !i2c_read_register(device->bus, device->adress, MAX17261_REG_VOLT_CELL, value, sizeof(value));
}

bool max17261_read_temperature(max17261_device_t *device, uint8_t *value,
		size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size >= 2, DART_ERROR_INVALID_VALUE, false);

	return i2c_read_register(device->bus, device->adress, MAX17261_REG_TEMP, value, sizeof(value));
}

bool max17261_read_current(max17261_device_t *device, uint8_t *value, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(size >= 2, DART_ERROR_INVALID_VALUE, false);

	return i2c_read_register(device->bus, device->adress, MAX17261_REG_CURRENT, value, sizeof(value));
}
