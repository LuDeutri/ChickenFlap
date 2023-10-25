#ifndef DART_LEGACY_CAN_CONSTRUCT_H_
#define DART_LEGACY_CAN_CONSTRUCT_H_

/**
 * Helper function to construct CAN messages.
 * Construct a CAN message by putting data into the temporary helper buffer and send it.
 */

#ifdef HAL_ENABLE_LEGACY

#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/math.h"
#include "../../../chickenFlap/dart/util/fixpoint.h"

#include "../../../chickenFlap/dart/can.h"
#include "../../../chickenFlap/dart/analog.h"

#include "../../../chickenFlap/dart/legacy/adc1023.h"

/**
 * First data need to be saved in the to the helper buffer via the functions
 * construct_putUInt8, construct_putUInt16 and construct_fetchADC1023.
 * When the data is filled with the necessary data a CAN message can be sent via
 * the construct_sendCAN function.
 * Ensure that the dataBufferIndex value does not overlap with existing data
 * in the same CAN message.
 */

/**
 * Copies the value to can_dataBuffer beginning at the index dataBufferIndex. See can_dataBuffer.
 * dataBufferIndex MUST be less than CAN_MAX_DATA_SIZE.
 */
void construct_putUInt8(uint8_t dataBufferIndex, uint8_t value);

/**
 * Copies the value to can_dataBuffer beginning at the index dataBufferIndex. See can_dataBuffer.
 * dataBufferIndex MUST be less than (CAN_MAX_DATA_SIZE - 1).
 * Data is saved with the format: Upper Byte | Lower Byte (Big-Endian)
 */
void construct_putInt16(uint8_t dataBufferIndex, int16_t value);

/**
 * Copies the value to can_dataBuffer beginning at the index dataBufferIndex. See can_dataBuffer.
 * dataBufferIndex MUST be less than (CAN_MAX_DATA_SIZE - 1).
 * Data is saved with the format: Upper Byte | Lower Byte (Big-Endian)
 */
void construct_putUInt16(uint8_t dataBufferIndex, uint16_t value);

/**
 * Copies the value to can_dataBuffer beginning at the index dataBufferIndex. See can_dataBuffer.
 * dataBufferIndex MUST be less than (CAN_MAX_DATA_SIZE - 1).
 */
void construct_putUInt32(uint8_t dataBufferIndex, uint32_t value);

/**
 * Gets the raw ADC value in the interval [0, 1023] from the specified channel and copies
 * the value to can_dataBuffer beginning at the index dataBufferIndex. See putUInt16, getADC1023.
 * dataBufferIndex MUST be less than (CAN_MAX_DATA_SIZE - 1).
 * The raw ADC value is returned.
 */
uint16_t construct_fetchADC1023(uint8_t dataBufferIndex, uint8_t channel);

/**
 * Gets the raw ADC value in the interval [0, 1023] from the specified channel and copies
 * the value as fixpoint to can_dataBuffer beginning at the index dataBufferIndex. See putUInt16, getADC1023.
 * dataBufferIndex MUST be less than (CAN_MAX_DATA_SIZE - 1).
 * The raw fixpoint ADC value is returned.
 */
uint16_t construct_fetchADC1023Fixpoint(uint8_t dataBufferIndex, uint8_t channel, float offset, float factor, uint8_t n);


/**
 * Sends the CANMessage message with the data saved in the helper buffer can_dataBuffer.
 * See putUInt8, putUInt16 and can_dataBuffer.
 */
void construct_sendCAN(CANMessage_t message);

#endif
#endif
