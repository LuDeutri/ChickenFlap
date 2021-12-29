/*
 * Configuration file for the AMS master 2019 r1 board.
 */

#ifdef USE_BOARD_TMS320_LAUNCHPAD
#ifndef BOARD_TMS320_LAUNCHPAD
#define BOARD_TMS320_LAUNCHPAD

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "TMS320_LAUNCHPAD"

// Include any HAL definitions

// HAL config
#define USE_TMS320

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
//#define HAL_ENABLE_UART
//#define HAL_ENABLE_I2C
#define HAL_ENABLE_SPI
//#define HAL_ENABLE_FILESYSTEM
//#define HAL_ENABLE_NETWORK
//#define HAL_ENABLE_HIGHRES_TIME
//#define HAL_ENABLE_RTC
//#define HAL_ENABLE_FLASH

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
//#define HAL_DUMMY_RTC

//#define HAL_ENABLE_CAN_MCP2515
//#define HAL_ENABLE_NETWORK_LWIP
//#define HAL_ENABLE_FILESYSTEM_FATFS

// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 21

// Logical and physical name already match for TMS570, so no mapping is done
// Constants for mapping the physical name to logical ADC channel.
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3
#define ADC_CHANNEL_4 4
#define ADC_CHANNEL_5 5
#define ADC_CHANNEL_6 6
#define ADC_CHANNEL_7 7
#define ADC_CHANNEL_8 8
#define ADC_CHANNEL_9 9
#define ADC_CHANNEL_10 10
#define ADC_CHANNEL_11 11
#define ADC_CHANNEL_12 12
#define ADC_CHANNEL_13 13
#define ADC_CHANNEL_14 14
#define ADC_CHANNEL_15 15
#define ADC_CHANNEL_16 16
#define ADC_CHANNEL_17 17
#define ADC_CHANNEL_18 18
#define ADC_CHANNEL_19 19
#define ADC_CHANNEL_20 20

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 168

// Define debug pins
#define DIGITAL_PIN_LED1 31
#define DIGITAL_PIN_LED2 34

// CAN
#define CAN_MAP { NULL, NULL, NULL }

// Override CAN settings
// #define CAN_MAX_RECEIVE_CALLBACKS 128
// #define CAN_TRANSMIT_BUFFER_SIZE 128
// #define CAN_RECEIVE_BUFFER_SIZE 256
// #define CANDATA_ENABLE_FAST_PATH

// UART
#define UART_COUNT 0

#define UART_MAP {}

// Define debug UART
// #define UART_LOG

// SPI
#define SPI_COUNT 0
#define SPI_MAP { }

// I2C
#define I2C_COUNT 0
#define I2C_MAP {}


#endif
#endif
