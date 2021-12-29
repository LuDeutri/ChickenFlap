/**
 * Configuration file for the ASSI controller.
 */

#ifdef USE_BOARD_ASSI_CONTROLLER_R1
#ifndef BOARD_ASSI_CONTROLLER
#define BOARD_ASSI_CONTROLLER

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
#define HAL_BOARD "STM32F103_ASSI_CONTROLLER_R1"

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
//#define HAL_ENABLE_I2C
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


// Externs for the variables in main.c
extern CAN_HandleTypeDef hcan;
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

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 7

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOC }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_13 }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PA1 0
#define DIGITAL_PA2 1
#define DIGITAL_PA6 2
#define DIGITAL_PA7 3
#define DIGITAL_PB0 4
#define DIGITAL_PB1 5
#define DIGITAL_PC13 6

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

// Define debug pins
#define DIGITAL_LED_GREEN DIGITAL_PC13

// CAN
#define CAN_MAP { &hcan, NULL, NULL }
#define CAN_1 0

// UART
#define UART_COUNT 1
#define UART_MAP { &huart1 }

#define DART_UART1 0
// Define debug UART
#define UART_LOG DART_UART1

// SPI
#define SPI_COUNT 0
// I2C
#define I2C_COUNT 0

#endif
#endif
