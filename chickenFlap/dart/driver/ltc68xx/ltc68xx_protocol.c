#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_protocol.h"

/**
 * Buffer to temporarily construct the data sent to the slaves over the SPI bus or receive the data received by the slaves over SPI.
 */
uint8_t ltc68xx_buffer[256];

bool ltc68xx_cmd_hold_clockcycles(ltc68xx_chain_t* chain, uint16_t cmd, size_t dummyBytesToRead) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(dummyBytesToRead < sizeof(ltc68xx_buffer), DART_ERROR_INVALID_VALUE, false);

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
    // Some commands need the SPI bus to be still
    if (dummyBytesToRead > 0) {
        if (!spi_read(chain->bus, ltc68xx_buffer, dummyBytesToRead))
            return false;
    }
    if (!spi_end(chain->bus))
        return false;
    digitalWrite(chain->cs, HIGH);
    return true;
}

bool ltc68xx_cmd(ltc68xx_chain_t* chain, uint16_t cmd) {
    return ltc68xx_cmd_hold_clockcycles(chain, cmd, 0);
}

bool ltc68xx_read(ltc68xx_chain_t* chain, uint16_t cmd) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(LTC68XX_BUFFER_OFFSET(chain->numICs) <= sizeof(ltc68xx_buffer), DART_ERROR_INVALID_VALUE, false);

    uint8_t txData[4];
    txData[0] = cmd >> 8;
    txData[1] = cmd & 0xFF;

    uint16_t pec = pec15_calc(txData, 2);
    txData[2] = pec >> 8;
    txData[3] = pec & 0xFF;

    // Clear buffer before reading
    memset(ltc68xx_buffer, 0, sizeof(ltc68xx_buffer));

    digitalWrite(chain->cs, LOW);
    if (!spi_begin(chain->bus))
        return false;
    if (!spi_write(chain->bus, txData, sizeof(txData)))
        return false;
    if (!spi_read(chain->bus, ltc68xx_buffer, LTC68XX_BUFFER_OFFSET(chain->numICs)))
        return false;
    if (!spi_end(chain->bus))
        return false;
    digitalWrite(chain->cs, HIGH);
    return true;
}

bool ltc68xx_write(ltc68xx_chain_t* chain, uint16_t cmd) {
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
    if (!spi_write(chain->bus, ltc68xx_buffer, (LTC68XX_BYTES_PER_REGISTER_GROUP + 2) * chain->numICs))
        return false;
    if (!spi_end(chain->bus))
        return false;
    digitalWrite(chain->cs, HIGH);
    return true;
}

bool ltc68xx_check_pec(ltc68xx_chain_t* chain, size_t index) {
    DART_ASSERT_RETURN(index < chain->numICs, DART_ERROR_INVALID_VALUE, false);

    ltc68xx_t* device = &chain->devices[index];

    // Compare sent PEC with calculated
    uint16_t sentPEC = (ltc68xx_buffer[LTC68XX_BUFFER_PEC(index)] << 8) | ltc68xx_buffer[LTC68XX_BUFFER_PEC(index) + 1];
    bool ok = pec15_calc(ltc68xx_buffer + LTC68XX_BUFFER_OFFSET(index), LTC68XX_BYTES_PER_REGISTER_GROUP) == sentPEC;

    if(ok) {
        if (device->pec_error_counter > 0) // prevent underflow
            device->pec_error_counter--;
    }
    else {
        if (device->pec_error_counter < 255) // prevent overflow
            device->pec_error_counter++;
    }

    device->error.pec_error = device->pec_error_counter > 10;
    return ok;
}

bool ltc68xx_read_cmd(ltc68xx_chain_t* chain, uint16_t cmd, size_t offsetInStruct) {
    // Ensure offset is actually in structure
    DART_ASSERT_RETURN(offsetInStruct <= sizeof(ltc68xx_t), DART_ERROR_INVALID_VALUE, false);

    // Perform read command
    if (!ltc68xx_read(chain, cmd))
        return false;

    // Copy data from LTC buffer to device structure
    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[i];
        DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

        // Check PEC
        if (!ltc68xx_check_pec(chain, i))
            continue;

        // Copy data in the structure
        memcpy(((uint8_t*)device) + offsetInStruct, ltc68xx_buffer + LTC68XX_BUFFER_OFFSET(i), LTC68XX_BYTES_PER_REGISTER_GROUP);
    }
    return true;
}


void ltc68xx_write_pec(size_t index) {
    DART_ASSERT(LTC68XX_BUFFER_PEC(index) + 1 < sizeof(ltc68xx_buffer), DART_ERROR_INVALID_VALUE);

    uint16_t pec = pec15_calc(ltc68xx_buffer + LTC68XX_BUFFER_OFFSET(index), LTC68XX_BYTES_PER_REGISTER_GROUP);
    ltc68xx_buffer[LTC68XX_BUFFER_PEC(index)] = pec >> 8;
    ltc68xx_buffer[LTC68XX_BUFFER_PEC(index) + 1] = pec & 0xFF;
}

bool ltc68xx_write_cmd(ltc68xx_chain_t* chain, uint16_t cmd, size_t offsetInStruct) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);

    // Ensure offset is actually in structure
    DART_ASSERT_RETURN(offsetInStruct <= sizeof(ltc68xx_t), DART_ERROR_INVALID_VALUE, false);

    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_t* device = &chain->devices[chain->numICs - i - 1]; // Order is reversed (first slave data is sent at the end)
        DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

        memcpy(ltc68xx_buffer + LTC68XX_BUFFER_OFFSET(i), ((uint8_t*)device) + offsetInStruct, LTC68XX_BYTES_PER_REGISTER_GROUP);
        ltc68xx_write_pec(i);
    }
    return ltc68xx_write(chain, cmd);
}

// This are small wrapper functions for reading and writing specific parts of the registers.
bool ltc68xx_wakeup(ltc68xx_chain_t* chain) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);

    // Produce some communication so ICs wake up (received data is discarded)
    return ltc68xx_read(chain, LTC68XX_CMD_RDCFGA);
}

bool ltc68xx_read_sid(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDSID, offsetof(ltc68xx_t, sid));
}

bool ltc68xx_read_status_a(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDSTATA, offsetof(ltc68xx_t, status_a));
}

bool ltc68xx_read_status_b(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDSTATB, offsetof(ltc68xx_t, status_b));
}

bool ltc68xx_read_cellVoltage_a(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCVA, offsetof(ltc68xx_t, cellVoltage_a));
}

bool ltc68xx_read_cellVoltage_b(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCVB, offsetof(ltc68xx_t, cellVoltage_b));
}

bool ltc68xx_read_cellVoltage_c(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCVC, offsetof(ltc68xx_t, cellVoltage_c));
}

bool ltc68xx_read_cellVoltage_d(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCVD, offsetof(ltc68xx_t, cellVoltage_d));
}

bool ltc68xx_read_cellVoltage_e(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCVE, offsetof(ltc68xx_t, cellVoltage_e));
}

bool ltc68xx_read_cellVoltage_f(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCVF, offsetof(ltc68xx_t, cellVoltage_f));
}

bool ltc68xx_read_aux_a(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDAUXA, offsetof(ltc68xx_t, aux_a));
}

bool ltc68xx_read_aux_b(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDAUXB, offsetof(ltc68xx_t, aux_b));
}

bool ltc68xx_read_config_a(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCFGA, offsetof(ltc68xx_t, config_a_rx));
}

bool ltc68xx_read_config_b(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCFGB, offsetof(ltc68xx_t, config_b_rx));
}

bool ltc68xx_read_comm(ltc68xx_chain_t* chain) {
    return ltc68xx_read_cmd(chain, LTC68XX_CMD_RDCOMM, offsetof(ltc68xx_t, commRx));
}

bool ltc68xx_write_config_a(ltc68xx_chain_t* chain) {
    return ltc68xx_write_cmd(chain, LTC68XX_CMD_WRCFGA, offsetof(ltc68xx_t, config_a));
}

bool ltc68xx_write_config_b(ltc68xx_chain_t* chain) {
    return ltc68xx_write_cmd(chain, LTC68XX_CMD_WRCFGB, offsetof(ltc68xx_t, config_b));
}

bool ltc68xx_write_comm(ltc68xx_chain_t* chain) {
    return ltc68xx_write_cmd(chain, LTC68XX_CMD_WRCOMM, offsetof(ltc68xx_t, commTx));
}

// Broadcast commands
bool ltc68xx_cmd_ADCV(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, bool dcp, uint8_t ch) {
    uint16_t cmd = LTC68XX_CMD_ADCV | (ltc68xx_convert_adc_mode(mode) << 7) | (dcp << 4) | ch;
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_ADAX(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chg) {
    // Is ADAX on LTC6804 devices
    uint16_t cmd = LTC68XX_CMD_ADAX | (ltc68xx_convert_adc_mode(mode) << 7) | chg;
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_ADAXD(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chg) {
    // Is ADAX on LTC6804 devices
    uint16_t cmd = LTC68XX_CMD_ADAXD | (ltc68xx_convert_adc_mode(mode) << 7) | chg;
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_ADSTAT(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chst) {
    uint16_t cmd = LTC68XX_CMD_ADSTAT | (ltc68xx_convert_adc_mode(mode) << 7) | chst;
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_ADSTATD(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chst) {
    uint16_t cmd = LTC68XX_CMD_ADSTATD | (ltc68xx_convert_adc_mode(mode) << 7) | chst;
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_AXST(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t st) {
    uint16_t cmd = LTC68XX_CMD_AXST | (ltc68xx_convert_adc_mode(mode) << 7) | (st << 5);
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_CVST(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t st) {
    uint16_t cmd = LTC68XX_CMD_CVST | (ltc68xx_convert_adc_mode(mode) << 7) | (st << 5);
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_STATST(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t st) {
    uint16_t cmd = LTC68XX_CMD_STATST | (ltc68xx_convert_adc_mode(mode) << 7) | (st << 5);
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_cmd_ADOW(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, bool pullup, bool dcp, uint8_t ch) {
    uint16_t cmd = LTC68XX_CMD_ADOW | (ltc68xx_convert_adc_mode(mode) << 7) | (pullup << 6) | (dcp << 4) | ch;
    return ltc68xx_cmd(chain, cmd);
}

bool ltc68xx_read_status(ltc68xx_chain_t* chain) {
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_read_cmd(chain, LTC68XX_CMD_RDSTATA, offsetof(ltc68xx_t, status_a)))
        return false;
    if (!ltc68xx_read_cmd(chain, LTC68XX_CMD_RDSTATB, offsetof(ltc68xx_t, status_b)))
        return false;
    return true;
}

bool ltc68xx_read_aux(ltc68xx_chain_t* chain) {
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_read_aux_a(chain))
        return false;
    if (!ltc68xx_read_aux_b(chain))
        return false;
    return true;
}


bool ltc68xx_read_cell_voltages(ltc68xx_chain_t* chain) {
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_read_cellVoltage_a(chain))
        return false;
    if (!ltc68xx_read_cellVoltage_b(chain))
        return false;
    if (!ltc68xx_read_cellVoltage_c(chain))
        return false;
    if (!ltc68xx_read_cellVoltage_d(chain))
        return false;
    if (chain->needLTC6813Registers) {
        if (!ltc68xx_read_cellVoltage_e(chain))
            return false;
        if (!ltc68xx_read_cellVoltage_f(chain))
            return false;
    }
    return true;
}

bool ltc68xx_read_config(ltc68xx_chain_t* chain) {
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_read_config_a(chain))
        return false;
    if (chain->needLTC6813Registers && !ltc68xx_read_config_b(chain))
        return false;
    return true;
}


bool ltc68xx_write_config(ltc68xx_chain_t* chain) {
    if (!ltc68xx_wakeup(chain))
        return false;
    if (!ltc68xx_write_config_a(chain))
        return false;
    if (chain->needLTC6813Registers && !ltc68xx_write_config_b(chain))
        return false;
    return true;
}
