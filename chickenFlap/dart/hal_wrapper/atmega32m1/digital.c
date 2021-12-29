#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/digital.h"

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_GPIO)

#ifndef DIGITAL_CHANNEL_COUNT
#error "DIGITAL_CHANNEL_COUNT is not defined"
#endif

#ifndef DIGITAL_MAP_PERIPHERAL
#error "DIGITAL_MAP_PERIPHERAL is not defined"
#endif

#ifndef DIGITAL_MAP_CHANNEL
#error "DIGITAL_MAP_CHANNEL is not defined"
#endif

char digitalPeripheralMap[DIGITAL_CHANNEL_COUNT] = DIGITAL_MAP_PERIPHERAL;
uint16_t digitalChannelMap[DIGITAL_CHANNEL_COUNT] = DIGITAL_MAP_CHANNEL;

void dartDigital_init() {
	// Everything done in main.c
}

void dartDigital_update() {
	// Nothing to do
}

volatile uint8_t* _hal_digital_port(int channel) {
	switch(digitalPeripheralMap[channel]) {
		case 'B':
			return &PORTB;
		case 'C':
			return &PORTC;
		case 'D':
			return &PORTD;
	}
	return NULL;
}

volatile uint8_t* _hal_digital_ddr(int channel) {
	switch(digitalPeripheralMap[channel]) {
		case 'B':
			return &DDRB;
		case 'C':
			return &DDRC;
		case 'D':
			return &DDRD;
	}
	return NULL; 
}


void digitalMode(int channel, PinMode_t mode) {
	DART_ASSERT(channel >= 0, ERROR_HAL);
	DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, ERROR_HAL);

	volatile uint8_t* port = _hal_digital_port(channel);
	volatile uint8_t* ddr = _hal_digital_ddr(channel);
	DART_NOT_NULL(port, ERROR_HAL);
	DART_NOT_NULL(ddr, ERROR_HAL);
	
	int pin = digitalChannelMap[channel];
	switch(mode) {
	case ModeInput:
	case ModeInputPullDown:
		*ddr &= ~(1 << pin);
		*port &= ~(1 << pin); 
		break;
	case ModeInputPullUp:
		*ddr &= ~(1 << pin);
		*port |= (1 << pin); 
		break;
	case ModeOutput:
	case ModeOutputOpenDrain:
		*ddr |= (1 << pin);
		break;
	}
}

bool digitalRead(int channel) {
	DART_ASSERT_RETURN(channel >= 0, ERROR_HAL, false);
	DART_ASSERT_RETURN(channel < DIGITAL_CHANNEL_COUNT, ERROR_HAL, false);
	
	volatile uint8_t* port = _hal_digital_port(channel);
	DART_NOT_NULL_RETURN(port, ERROR_HAL, false);
	return (*port & digitalChannelMap[channel]) != 0;
}

void digitalWrite(int channel, bool state) {
	DART_ASSERT(channel >= 0, ERROR_HAL);
	DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, ERROR_HAL);
	
	volatile uint8_t* port = _hal_digital_port(channel);
	DART_NOT_NULL(port, ERROR_HAL);
	if (state)
		*port |= (1 << digitalChannelMap[channel]);
	else
		*port &= ~(1 << digitalChannelMap[channel]);
}

void digitalToggle(int channel) {
	DART_ASSERT(channel >= 0, ERROR_HAL);
	DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, ERROR_HAL);
	
	volatile uint8_t* port = _hal_digital_port(channel);
	DART_NOT_NULL(port, ERROR_HAL);
	*port ^= (1 << digitalChannelMap[channel]);
}

void digitatSetInterruptCallback(DigitalInterruptCallback callback, void* arg) {
    // Do nothing
}
#endif
