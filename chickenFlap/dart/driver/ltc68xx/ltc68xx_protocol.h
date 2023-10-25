#ifndef DART_DRIVER_LTC68XX_LTC68XX_PROTOCOL_H_
#define DART_DRIVER_LTC68XX_LTC68XX_PROTOCOL_H_

#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Calculates the index in ltc68xx_buffer for the given slave index i.
 */
#define LTC68XX_BUFFER_OFFSET(i) ((LTC68XX_BYTES_PER_REGISTER_GROUP + 2) * (i))
/**
 * Calculates the index of the PEC in ltc68xx_buffer for the given slave index i.
 */
#define LTC68XX_BUFFER_PEC(i) ((LTC68XX_BUFFER_OFFSET(i) + LTC68XX_BYTES_PER_REGISTER_GROUP))

/**
 * Send the given command to the LTC68XX chain and reading given dummy bytes from the SPI bus (doing clock cycles)
 */
bool ltc68xx_cmd_hold_clockcycles(ltc68xx_chain_t* chain, uint16_t cmd, size_t dummyBytesToRead);

bool ltc68xx_cmd(ltc68xx_chain_t* chain, uint16_t cmd);

/**
 * Reads data from the LTC68XX chain given the command.
 * The data is saved in the ltc68xx_buffer array.
 */
bool ltc68xx_read(ltc68xx_chain_t* chain, uint16_t cmd);

/**
 * Writes data from the LTC68XX chain given the command.
 * The data is read from the ltc68xx_buffer array.
 */
bool ltc68xx_write(ltc68xx_chain_t* chain, uint16_t cmd);

/**
 * Checks if the PEC is OK for the transmission by the slave at the given index.
 */
bool ltc68xx_check_pec(ltc68xx_chain_t* chain, size_t index);

/**
 * Initiates the given read command, and copies the receive data in the devie struct with the given offset.
 */
bool ltc68xx_read_cmd(ltc68xx_chain_t* chain, uint16_t cmd, size_t offsetInStruct);

/**
 * Calculates and writes the PEC for a slave at the given index.
 */
void ltc68xx_write_pec(size_t index);

/**
 * Initiates the given write command, and copies the data to write from the devie struct with the given offset to the LTC buffer.
 */
bool ltc68xx_write_cmd(ltc68xx_chain_t* chain, uint16_t cmd, size_t offsetInStruct);

// This are small wrapper functions for reading and writing specific parts of the registers.
bool ltc68xx_wakeup(ltc68xx_chain_t* chain);
bool ltc68xx_read_sid(ltc68xx_chain_t* chain);
bool ltc68xx_read_status_a(ltc68xx_chain_t* chain);
bool ltc68xx_read_status_b(ltc68xx_chain_t* chain);
bool ltc68xx_read_cellVoltage_a(ltc68xx_chain_t* chain);
bool ltc68xx_read_cellVoltage_b(ltc68xx_chain_t* chain);
bool ltc68xx_read_cellVoltage_c(ltc68xx_chain_t* chain);
bool ltc68xx_read_cellVoltage_d(ltc68xx_chain_t* chain);
bool ltc68xx_read_cellVoltage_e(ltc68xx_chain_t* chain);
bool ltc68xx_read_cellVoltage_f(ltc68xx_chain_t* chain);
bool ltc68xx_read_aux_a(ltc68xx_chain_t* chain);
bool ltc68xx_read_aux_b(ltc68xx_chain_t* chain);
bool ltc68xx_read_config_a(ltc68xx_chain_t* chain);
bool ltc68xx_read_comm(ltc68xx_chain_t* chain);
bool ltc68xx_write_config_a(ltc68xx_chain_t* chain);
bool ltc68xx_write_comm(ltc68xx_chain_t* chain);

// Broadcast commands
bool ltc68xx_cmd_ADCV(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, bool dcp, uint8_t ch);
bool ltc68xx_cmd_ADAX(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chg);
bool ltc68xx_cmd_ADAXD(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chg);
bool ltc68xx_cmd_ADSTAT(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chst);
bool ltc68xx_cmd_ADSTATD(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t chst);
bool ltc68xx_cmd_AXST(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t st);
bool ltc68xx_cmd_CVST(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t st);
bool ltc68xx_cmd_STATST(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, uint8_t st);
bool ltc68xx_cmd_ADOW(ltc68xx_chain_t* chain, ltc68xx_adc_mode_t mode, bool pullup, bool dcp, uint8_t ch);

// Read multiple registers at once
bool ltc68xx_read_status(ltc68xx_chain_t* chain);
bool ltc68xx_read_aux(ltc68xx_chain_t* chain);
bool ltc68xx_read_cell_voltages(ltc68xx_chain_t* chain);
bool ltc68xx_read_config(ltc68xx_chain_t* chain);

/**
 * Writes the locally saved configuration to the chain.
 */
bool ltc68xx_write_config(ltc68xx_chain_t* chain);

#ifdef __cplusplus
}
#endif

#endif
