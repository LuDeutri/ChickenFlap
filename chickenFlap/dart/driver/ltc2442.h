#ifndef DART_DRIVER_LTC2442_H_
#define DART_DRIVER_LTC2442_H_

#include "../../../chickenFlap/dart/spi.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    /**
     * The SPI bus to use.
     */
    int spi;
    /**
     * The chip select digital pin to use.
     */
    int cs;
    /**
     * The miso digital pin to use for checking if the LTC2442 is done with conversion. Can be -1 to use a simple delay.
     */
    int miso;

    util_time_t lastConversion;
} ltc2442_device_t;

#define LTC2442_CMD_SAMPLE       0x4000
#define LTC2442_CMD_CHANGE_INPUT 0x5000

#define LTC2442_OSR_64         1
#define LTC2442_OSR_128        2
#define LTC2442_OSR_256        3
#define LTC2442_OSR_512        4
#define LTC2442_OSR_1024       5
#define LTC2442_OSR_2048       6
#define LTC2442_OSR_4096       7
#define LTC2442_OSR_8192       8
#define LTC2442_OSR_16384      9
#define LTC2442_OSR_32768      10

/**
 * Maximum raw ADC value the LTC2442 returns.
 */
#define LTC2442_MAX_ADC_VALUE ((1 << 28) - 1)

bool ltc2442_init(ltc2442_device_t* device);

/**
 * Check if conversion is done.
 */
bool ltc2442_is_conversion_done(ltc2442_device_t* device, util_time_t timeout, bool* timeoutOccured);

/**
 * Reads the result from the ADC.
 */
bool ltc2442_read_result(ltc2442_device_t* device, int32_t* adcValue, bool* outOfRange);

/**
 * Commands the ADC to sample a new value. Will not update the ADC settings and use the last one set by ltc2442_sample_set().
 * Returns true when the command was successful or false otherwise.
 */
bool ltc2442_sample(ltc2442_device_t* device);

/**
 * Commands the ADC to sample a new value. Will update the ADC settings.
 * Returns true when the command was successful or false otherwise.
 *
 * singleEnd: Single-Ended Mode (when true)
 */
bool ltc2442_sample_set(ltc2442_device_t* device, bool singleEnd, bool odd, uint8_t address, uint8_t osr, bool twox);

#ifdef __cplusplus
}
#endif

#endif
