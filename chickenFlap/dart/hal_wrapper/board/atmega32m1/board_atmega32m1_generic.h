/**
 * Configuration file for the STM32F103 dev board.
 */

#ifdef USE_BOARD_ATMEGA32M1_GENERIC
#ifndef BOARD_ATMEGA32M1_GENERIC
#define BOARD_ATMEGA32M1_GENERIC

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif

#define DART_BOARD

/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "ATMEGA32M1_GENERIC"

// Include any HAL definitions
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#define FOSC 8000
#define F_CPU 8000000L
#define CAN_BAUDRATE 500

// HAL config
#define USE_ATMEGA32M1

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
//#define HAL_ENABLE_UART
#define HAL_ENABLE_I2C
//#define HAL_ENABLE_SPI
//#define HAL_ENABLE_FILESYSTEM
//#define HAL_ENABLE_NETWORK
//#define HAL_ENABLE_HIGHRES_TIME

//#define HAL_DUMMY_TIME
//#define HAL_DUMMY_CAN
//#define HAL_DUMMY_ADC
//#define HAL_DUMMY_GPIO
//#define HAL_DUMMY_UART
//#define HAL_DUMMY_I2C
//#define HAL_DUMMY_SPI
//#define HAL_DUMMY_FILESYSTEM
//#define HAL_DUMMY_NETWORK
//#define HAL_DUMMY_HIGHRES_TIME

// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 1023

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 10

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_ADC1 0
#define ANALOG_ADC2 1
#define ANALOG_ADC3 2
#define ANALOG_ADC4 3
#define ANALOG_ADC5 4
#define ANALOG_ADC6 5
#define ANALOG_ADC7 6
#define ANALOG_ADC8 7
#define ANALOG_ADC9 8
#define ANALOG_ADC10 9

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 21

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'D', 'D', 'D', 'D', 'D'  }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { PORTB0, PORTB1, PORTB2, PORTB3, PORTB4, PORTB5, PORTB6, PORTB7, PORTC0, PORTC1, PORTC2, PORTC3, PORTC4, PORTC5, PORTC6, PORTC7, PORTD0, PORTD1, PORTD5, PORTD6, PORTD7  }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PB0 0
#define DIGITAL_PB1 1
#define DIGITAL_PB2 2
#define DIGITAL_PB3 3
#define DIGITAL_PB4 4
#define DIGITAL_PB5 5
#define DIGITAL_PB6 6
#define DIGITAL_PB7 7
#define DIGITAL_PC0 8
#define DIGITAL_PC1 9
#define DIGITAL_PC2 10
#define DIGITAL_PC3 11
#define DIGITAL_PC4 12
#define DIGITAL_PC5 13
#define DIGITAL_PC6 14
#define DIGITAL_PC7 15
#define DIGITAL_PD0 16
#define DIGITAL_PD1 17
#define DIGITAL_PD5 18
#define DIGITAL_PD6 19
#define DIGITAL_PD7 20

// UART
#define UART_COUNT 0
#define UART_MAP { }

// SPI
#define SPI_COUNT 0
#define SPI_MAP { }

// I2C
#define I2C_COUNT 1

#define I2C_DATA_DDR DDRC
#define I2C_DATA_PIN PINC
#define I2C_DATA_PORT PORTC
#define I2C_DATA_PIN_NUMBER PORTC0

#define I2C_CLOCK_DDR DDRD
#define I2C_CLOCK_PIN PIND
#define I2C_CLOCK_PORT PORTD
#define I2C_CLOCK_PIN_NUMBER PORTD0

#endif
#endif
