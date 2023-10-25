#include "../../../../chickenFlap/dart/hal_wrapper/stm32/digital.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_GPIO)

#ifndef DIGITAL_CHANNEL_COUNT
#error "DIGITAL_CHANNEL_COUNT is not defined"
#endif

#ifndef DIGITAL_MAP_PERIPHERAL
#error "DIGITAL_MAP_PERIPHERAL is not defined"
#endif

#ifndef DIGITAL_MAP_CHANNEL
#error "DIGITAL_MAP_CHANNEL is not defined"
#endif

GPIO_TypeDef* digitalPeripheralMap[DIGITAL_CHANNEL_COUNT] = DIGITAL_MAP_PERIPHERAL;
uint16_t digitalChannelMap[DIGITAL_CHANNEL_COUNT] = DIGITAL_MAP_CHANNEL;

void dartDigital_init() {
	// Everything done in main.c
}

void dartDigital_update() {
	// Nothing to do
}

void digitalMode(int channel, PinMode_t mode) {
	DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE);

	GPIO_InitTypeDef gpio;
	gpio.Pin = digitalChannelMap[channel];
	switch(mode) {
	case ModeInput:
		gpio.Mode = GPIO_MODE_INPUT;
		gpio.Pull = GPIO_NOPULL;
		break;
	case ModeInputPullDown:
		gpio.Mode = GPIO_MODE_INPUT;
		gpio.Pull = GPIO_PULLDOWN;
		break;
	case ModeInputPullUp:
		gpio.Mode = GPIO_MODE_INPUT;
		gpio.Pull = GPIO_PULLUP;
		break;
	case ModeOutput:
		gpio.Mode = GPIO_MODE_OUTPUT_PP;
		gpio.Pull = GPIO_NOPULL;
		break;
	case ModeOutputOpenDrain:
        gpio.Mode = GPIO_MODE_OUTPUT_OD;
        gpio.Pull = GPIO_NOPULL;
        break;
	}
	gpio.Speed = DIGITAL_IO_SPEED;

	HAL_GPIO_Init(digitalPeripheralMap[channel], &gpio);
}

bool digitalRead(int channel) {
	DART_ASSERT_RETURN(channel >= 0, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE, false);

	return HAL_GPIO_ReadPin(digitalPeripheralMap[channel], digitalChannelMap[channel]) == GPIO_PIN_SET;
}

void digitalWrite(int channel, bool state) {
	DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE);

	HAL_GPIO_WritePin(digitalPeripheralMap[channel], digitalChannelMap[channel], state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void digitalToggle(int channel) {
	DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE);

	HAL_GPIO_TogglePin(digitalPeripheralMap[channel], digitalChannelMap[channel]);
}

void digitatSetInterruptCallback(DigitalInterruptCallback callback, void* arg) {
	DART_NOT_USED(callback);
	DART_NOT_USED(arg);
    // Do nothing
}
#endif
