#ifndef DART_LEGACY_ADC1023_H_
#define DART_LEGACY_ADC1023_H_

#ifdef HAL_ENABLE_LEGACY

#include "../../../chickenFlap/dart/analog.h"

#define ADC1023_MAX_VALUE 1023

uint16_t getADC1023(uint8_t channel);

#endif

#endif
