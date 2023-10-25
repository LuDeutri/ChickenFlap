#ifndef DART_DRIVER_TLC59116_H_
#define DART_DRIVER_TLC59116_H_

#include "../../../chickenFlap/dart/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TLC59116_LED_COUNT 16

#define TLC59116_SLAVE_ADDRESS_0 (0b1100000)

#define TLC59116_REG_MODE1 0x0
#define TLC59116_REG_MODE2 0x1
#define TLC59116_REG_PWM0 0x2
#define TLC59116_REG_GRPPWM 0x12
#define TLC59116_REG_GRPFREQ 0x13
#define TLC59116_REG_LEDOUT0 0x14
#define TLC59116_REG_SUBADR1 0x18
#define TLC59116_REG_SUBADR2 0x19
#define TLC59116_REG_SUBADR3 0x1A
#define TLC59116_REG_ALLCALLADR 0x1B
#define TLC59116_REG_IREF 0x1C
#define TLC59116_REG_EFLAG1 0x1D

#define TLC59116_AUTO_INCREMENT 0x80

#define TLC59116_MODE1_CONFIG 0x90
#define TLC59116_MODE1_DISABLE_OSC (1 << 4)

#define TLC59116_MODE2_CONFIG 0
#define TLC59116_MODE2_CLEAR_ERR_FLAGS (1 << 7)

#define TLC59116_LED_MODE_OFF 0
#define TLC59116_LED_MODE_ON 1
#define TLC59116_LED_MODE_PWM 2
#define TLC59116_LED_MODE_PWM_BLINKING 3
#define TLC59116_LED_MODE_CLEAR 3

typedef struct {
	int bus;
	uint8_t device;

	uint8_t mode1;
	uint8_t mode2;
	uint8_t ledout[4];
} tlc59116_device_t;

bool tlc59116_init(tlc59116_device_t* device);

bool tlc59116_enable(tlc59116_device_t* device);
bool tlc59116_disable(tlc59116_device_t* device);
bool tlc59116_clear_error_flags(tlc59116_device_t* device);
bool tlc59116_read_error_flags(tlc59116_device_t* device, bool* led_errors, size_t size);
bool tlc59116_check_and_clear_any_led_error(tlc59116_device_t* device);

bool tlc59116_write_led(tlc59116_device_t* device, uint8_t index, float brightness);

#ifdef __cplusplus
}
#endif


#endif
