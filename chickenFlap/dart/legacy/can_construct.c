#include "../../../chickenFlap/dart/legacy/can_construct.h"

#ifdef HAL_ENABLE_LEGACY

/**
 * Buffer to save the constructed message.
 * Data is put in via construct_putUInt8() and then send via construct_sendCAN().
 */
uint8_t can_dataBuffer[CAN_MAX_DATA_SIZE];

void construct_putUInt8(uint8_t dataBufferIndex, uint8_t value) {
    DART_ASSERT(dataBufferIndex < sizeof(can_dataBuffer), DART_ERROR_INVALID_VALUE);
    can_dataBuffer[dataBufferIndex] = value;
}

void construct_putInt16(uint8_t dataBufferIndex, int16_t value) {
    // The value is saved with 2 bytes in the helper buffer, thus
    // we need 2 bytes of space in the buffer.
    DART_ASSERT(dataBufferIndex < (sizeof(can_dataBuffer) - 1), DART_ERROR_INVALID_VALUE);

    // Copy first upper half then the lower half.
    construct_putUInt8(dataBufferIndex, upper_byte(value));
    construct_putUInt8(dataBufferIndex + 1, lower_byte(value));
}

void construct_putUInt16(uint8_t dataBufferIndex, uint16_t value) {
    // The value is saved with 2 bytes in the helper buffer, thus
    // we need 2 bytes of space in the buffer.
    DART_ASSERT(dataBufferIndex < (sizeof(can_dataBuffer) - 1), DART_ERROR_INVALID_VALUE);

    // Copy first upper half then the lower half.
    construct_putUInt8(dataBufferIndex, upper_byte(value));
    construct_putUInt8(dataBufferIndex + 1, lower_byte(value));
}

void construct_putUInt32(uint8_t dataBufferIndex, uint32_t value) {
    // The value is saved with 4 bytes in the helper buffer, thus
    // we need 4 bytes of space in the buffer.
    DART_ASSERT(dataBufferIndex < (sizeof(can_dataBuffer) - 3), DART_ERROR_INVALID_VALUE);

    construct_putUInt8(dataBufferIndex, (value >> 24) & 0xFF);
    construct_putUInt8(dataBufferIndex + 1, (value >> 16) & 0xFF);
    construct_putUInt8(dataBufferIndex + 2, (value >> 8) & 0xFF);
    construct_putUInt8(dataBufferIndex + 3, value & 0xFF);
}

uint16_t construct_fetchADC1023(uint8_t dataBufferIndex, uint8_t channel) {
    uint16_t value = getADC1023(channel);
    construct_putUInt16(dataBufferIndex, value);
    return value;
}

uint16_t construct_fetchADC1023Fixpoint(uint8_t dataBufferIndex, uint8_t channel, float offset, float factor, uint8_t n) {
    uint16_t value = convertToFixpoint(((getADC1023(channel) / (float)ADC1023_MAX_VALUE) - offset) * factor, n);
    construct_putUInt16(dataBufferIndex, value);
    return value;
}

void construct_sendCAN(CANMessage_t message) {
    // Ensure our buffer had actually enough space to save the full message
    DART_ASSERT(sizeof(can_dataBuffer) >= message.dataLength, DART_ERROR_BUFFER_OVERFLOW);

    // Because sendCAN excepts the size to always match the dataLength of the message
    // we use dataLength and not sizeof(can_dataBuffer), because
    // the buffer size may be larger than the sent message.
    // See can_send in "can.h".
    can_send(message, can_dataBuffer, message.dataLength);
}
#endif
