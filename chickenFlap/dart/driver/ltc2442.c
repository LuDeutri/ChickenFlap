#include "../../../chickenFlap/dart/driver/ltc2442.h"

#ifdef __cplusplus
extern "C" {
#endif


bool ltc2442_init(ltc2442_device_t* device) {
    DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
    device->lastConversion = 0;
    return true;
}

bool ltc2442_is_conversion_done(ltc2442_device_t* device, util_time_t timeout, bool* timeoutOccured) {
    DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
    DART_NOT_NULL_RETURN(timeoutOccured, DART_ERROR_VALUE_IS_NULL, false);


    util_time_t t = millis() - device->lastConversion;
    if (t >= timeout) {
        *timeoutOccured = true;
        return false;
    }
    *timeoutOccured = false;

    // Wait till conversation is done
    if (device->miso >= 0) {
        digitalWrite(device->cs, LOW);
        bool done = digitalRead(device->miso) == LOW;
        digitalWrite(device->cs, HIGH);
        return done;
    }
    // do a simple wait when we do not have any MISO pin for checking the status
    return t >= 8; // wait for 8 ms
}

bool ltc2442_read_result(ltc2442_device_t* device, int32_t* adcValue, bool* outOfRange) {
    DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
    DART_NOT_NULL_RETURN(adcValue, DART_ERROR_VALUE_IS_NULL, false);
    DART_NOT_NULL_RETURN(outOfRange, DART_ERROR_VALUE_IS_NULL, false);

    *adcValue = 0;
    *outOfRange =  false;

    // Read conversation result
    digitalWrite(device->cs, LOW);
    uint8_t data[4];
    if (!spi_read(device->spi, data, sizeof(data))) {
        digitalWrite(device->cs, HIGH);
        return false;
    }
    digitalWrite(device->cs, HIGH);

    // Ensure the byte order is correct
    uint32_t rawValue = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];

    // Check if done
    bool done = (rawValue & (1u << 31u)) == 0;
    if (!done)
        return false;

    // Check if value is out of range
    bool sign = (rawValue & (1 << 29)) == 0;
    bool msb = (rawValue & (1 << 28)) == 0;
    *outOfRange = (sign == msb);

    // Prepare returned value
    *adcValue = (int32_t)(rawValue & LTC2442_MAX_ADC_VALUE);
    if (sign)
        *adcValue *= -1;
    return true;
}

bool _ltc2442_sample(ltc2442_device_t* device, uint16_t cmd) {
    DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

    // Ensure the byte order is correct
    uint8_t data[2];
    data[0] = cmd >> 8;
    data[1] = cmd & 0xFF;

    // Write command/trigger sample
    digitalWrite(device->cs, LOW);
    bool ok = spi_write(device->spi, data, sizeof(data));
    digitalWrite(device->cs, HIGH);
    device->lastConversion = millis();
    return ok;
}

bool ltc2442_sample(ltc2442_device_t* device) {
    return _ltc2442_sample(device, LTC2442_CMD_SAMPLE);
}

bool ltc2442_sample_set(ltc2442_device_t* device, bool singleEnd, bool odd, uint8_t address, uint8_t osr, bool twox) {
    DART_ASSERT_RETURN(address < 8, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(osr >= LTC2442_OSR_64 && osr <= LTC2442_OSR_32768, DART_ERROR_INVALID_VALUE, false);

    uint16_t cmd = LTC2442_CMD_CHANGE_INPUT | (singleEnd << 12) | (odd << 11) | (address << 8) | (osr << 4) | (twox << 3);
    return _ltc2442_sample(device, cmd);
}

#ifdef __cplusplus
}
#endif
