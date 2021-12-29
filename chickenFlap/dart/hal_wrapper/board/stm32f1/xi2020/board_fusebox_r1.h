/**
 * Configuration file for the fusebox.
 */

#ifdef USE_BOARD_FUSEBOX_R1
#ifndef BOARD_FUSEBOX_R1
#define BOARD_FUSEBOX_R1

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

#ifndef STM32F103xB
#error "No STM32F103xB target"
#endif


/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "STM32F103_FUSEBOX_R1"

// Include any HAL definitions
#include <stm32f1xx_hal.h>

// HAL config
#define USE_STM32
#define USE_STM32F1

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
#define HAL_ENABLE_CAN
//#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
#define HAL_ENABLE_UART
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
#define HAL_DUMMY_HIGHRES_TIME


// Externs for the variables in main.c
extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 0

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP {}

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS {}

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_13CYCLES_5

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 1

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOC }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_13 }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PC13 0

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

// Define debug pins
#define DIGITAL_LED_DEBUG DIGITAL_PC13

// CAN
#define CAN_MAP { &hcan, NULL, NULL }

// UART
#define UART_COUNT 1
#define UART_MAP { &huart1 }

#define CAN_1 0

#define DART_UART1 0
// Define debug UART
#define UART_LOG DART_UART1

// SPI
#define SPI_COUNT 0
#define SPI_MAP {}

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c1 }

#endif
#endif
