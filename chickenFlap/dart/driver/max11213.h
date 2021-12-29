#ifndef DART_DRIVER_MAX11213_H_
#define DART_DRIVER_MAX11213_H_

#include "../../../chickenFlap/dart/digital.h"
#include "../../../chickenFlap/dart/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int bus;
	int cs;
}max11213_device_t;
#define RegisterAddressStat1 0x00
#define RegisterAddressCtrl1 0x02
#define RegisterAddressCtrl2 0x04
#define RegisterAddressCrtl3 0x06
#define RegisterAddressData  0x08
#define RegisterAddressSOC 	 0x0a
#define RegisterAddressSGC   0x0c
#define RegisterAddressSCOC  0x0e
#define RegisterAddressSCGC  0x10

#define basicReadAddress	 0xc1
#define basicWriteAddress	 0xc0

#define basicConversion 	 0x80

#define standardConfig 		 0x1a
#define standardGainConfig   0x00

#define basicGPIOOutput 0xf0
#define GPIO1high 0x08
#define GPIO2high 0x04
#define GPIO3high 0x02
#define GPIO4high 0x01

/**
 * initalies a max11213_device
 */
bool max11213_init(max11213_device_t* device);
/**
 * @device handled device
 * @status of the max11213
 */
bool max11213_get_Status(max11213_device_t* device,uint8_t* status);
/**
 * @device handled device
 * @rate byte that defines corresponds to a configurable rate (see datasheet)
 */
bool max11213_start_conversion(max11213_device_t* device, uint8_t rate);
/**
 * @device handled device
 * @finished true if conversion is finished
 */
bool max11213_is_conversion_finished(max11213_device_t* device, bool* finished);
/**
 * @device handled device
 * @data converted ADC data
 */
bool max11213_get_data(max11213_device_t* device, uint16_t* data);
/**
 * @device handled device
 * @conig of GPIOs (register Crtl2)
 */
bool max11213_set_GPIOs(max11213_device_t* device, uint8_t* config);
/**
 * @device handled device
 * @config data for register Crtl1
 */
bool max11213_setConfig(max11213_device_t* device, uint8_t* config);
/**
 * @device handled device
 * @config data for register Crtl3
 */
bool max11213_setGain(max11213_device_t* device, uint8_t* config);
/**
 * @device handled device
 * @addr register address
 * @data place to save read data
 * @size size of data
 */
bool max11213_read_register(max11213_device_t* device,uint8_t addr, uint8_t* data, size_t size);
/**
 * @device handled device
 * @addr register address
 * @data place where command data is stored
 * @size size of data
 */
bool max11213_write_register(max11213_device_t* device, uint8_t addr, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif


#endif
