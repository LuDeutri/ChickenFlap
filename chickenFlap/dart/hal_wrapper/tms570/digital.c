#include "../../../../chickenFlap/dart/hal_wrapper/tms570/digital.h"

#if defined(USE_TMS570) && defined(HAL_ENABLE_GPIO)

#ifndef DIGITAL_CHANNEL_COUNT
#error "DIGITAL_CHANNEL_COUNT is not defined"
#endif

#ifndef DIGITAL_MAP_PERIPHERAL
#error "DIGITAL_MAP_PERIPHERAL is not defined"
#endif

#ifndef DIGITAL_MAP_CHANNEL
#error "DIGITAL_MAP_CHANNEL is not defined"
#endif

gioPORT_t* digitalPeripheralMap[DIGITAL_CHANNEL_COUNT] = DIGITAL_MAP_PERIPHERAL;
uint32 digitalChannelMap[DIGITAL_CHANNEL_COUNT] = DIGITAL_MAP_CHANNEL;

DigitalInterruptCallback interruptCallback;
void* interruptCallbackArg;

void dartDigital_init() {
    interruptCallback = NULL;
    interruptCallbackArg = NULL;

    gioInit();
    hetInit();
    linInit();

    edgeEnableNotification(hetREG1, 0);
}

void dartDigital_update() {
    // Nothing to do
}

void digitalMode(int channel, PinMode_t mode) {
    DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE);

    gioPORT_t* port = digitalPeripheralMap[channel];
    uint32_t pin = 1 << digitalChannelMap[channel];

    // ADC event pin needs extra handling
    if ((adcBASE_t*)port == adcREG1) {
        adcBASE_t* adc = (adcBASE_t*)port;
        switch(mode) {
        case ModeInput:
            adc->EVTDIR &= ~pin;
            adc->EVTDIS |= pin;
            adc->EVTPDR &= ~pin;
            break;
        case ModeInputPullDown:
            adc->EVTDIR &= ~pin;
            adc->EVTDIS &= ~pin;
            adc->EVTPSEL &= ~pin;
            adc->EVTPDR &= ~pin;
            break;
        case ModeInputPullUp:
            adc->EVTDIR &= ~pin;
            adc->EVTDIS &= ~pin;
            adc->EVTPSEL |= pin;
            adc->EVTPDR &= ~pin;
            break;
        case ModeOutput:
            adc->EVTDIR |= pin;
            adc->EVTDIS |= pin;
            adc->EVTPDR &= ~pin;
            break;
        case ModeOutputOpenDrain:
            adc->EVTDIR |= pin;
            adc->EVTDIS |= pin;
            adc->EVTPDR |= pin;
            break;
        }
    }
    else { // normal GPIO pin
        switch(mode) {
        case ModeInput:
            port->DIR &= ~pin;
            port->PULDIS |= pin;
            port->PDR &= ~pin;
            break;
        case ModeInputPullDown:
            port->DIR &= ~pin;
            port->PULDIS &= ~pin;
            port->PSL &= ~pin;
            port->PDR &= ~pin;
            break;
        case ModeInputPullUp:
            port->DIR &= ~pin;
            port->PULDIS &= ~pin;
            port->PSL |= pin;
            port->PDR &= ~pin;
            break;
        case ModeOutput:
            port->DIR |= pin;
            port->PULDIS |= pin;
            port->PDR &= ~pin;
            break;
        case ModeOutputOpenDrain:
            port->DIR |= pin;
            port->PULDIS |= pin;
            port->PDR |= pin;
            break;
        }
    }
}

bool digitalRead(int channel) {
    DART_ASSERT_RETURN(channel >= 0, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE, false);

    gioPORT_t* port = digitalPeripheralMap[channel];

    // ADC event pin needs extra handling
    if ((adcBASE_t*)port == adcREG1) {
        adcBASE_t* adc = (adcBASE_t*)port;
        return (adc->EVTOUT & (1 << digitalChannelMap[channel])) != 0;
    }

    // normal GPIO pin
    return gioGetBit(digitalPeripheralMap[channel], digitalChannelMap[channel]) != 0;
}

void digitalWrite(int channel, bool state) {
    DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE);

    gioPORT_t* port = digitalPeripheralMap[channel];

    // ADC event pin needs extra handling
    if ((adcBASE_t*)port == adcREG1) {
        adcBASE_t* adc = (adcBASE_t*)port;
        if (state)
            adc->EVTOUT |= (1 << digitalChannelMap[channel]);
        else
            adc->EVTOUT &= ~(1 << digitalChannelMap[channel]);
    }
    else { // normal GPIO pin
        if (state)
            port->DOUT |= (1 << digitalChannelMap[channel]);
        else
            port->DOUT &= ~(1 << digitalChannelMap[channel]);
    }
}

void digitalToggle(int channel) {
    DART_ASSERT(channel >= 0, DART_ERROR_INVALID_VALUE);
    DART_ASSERT(channel < DIGITAL_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(digitalPeripheralMap[channel], DART_ERROR_INVALID_VALUE);

    digitalWrite(channel, !digitalRead(channel));
}

void digitatSetInterruptCallback(DigitalInterruptCallback callback, void* arg) {
    interruptCallbackArg = arg;
    interruptCallback = callback;
}

void edgeNotification(hetBASE_t* hetREG, uint32 edge) {
    if (interruptCallback != NULL) {
        interruptCallback(interruptCallbackArg);
    }
}
#endif
