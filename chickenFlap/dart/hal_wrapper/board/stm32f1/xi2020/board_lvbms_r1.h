/**
 * Configuration file for the lvbms.
 */

#ifdef USE_BOARD_LVBMS_R1
#ifndef BOARD_LVBMS
#define BOARD_LVBMS

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
#define HAL_BOARD "lvbms_r1"

// Include any HAL definitions
#include <stm32f1xx_hal.h>

// HAL config
#define USE_STM32
#define USE_STM32F1

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
#define HAL_ENABLE_UART
#define HAL_ENABLE_I2C
#define HAL_ENABLE_SPI
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
extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

// Analog
/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 1

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP { &hadc1 }

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_4 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_ADC1_IN4 0

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_13CYCLES_5

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 8

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOC }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_12, GPIO_PIN_13 }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PA1 0
#define DIGITAL_PA2 1
#define DIGITAL_PA3 2
#define DIGITAL_PA5 3
#define DIGITAL_PA7 4
#define DIGITAL_PB8 5
#define DIGITAL_PB12 6
#define DIGITAL_PC13 7

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

// Define debug pins
#define STATUS_LED_RED DIGITAL_PA1
#define STATUS_LED_YELLOW DIGITAL_PA2
#define STATUS_LED_GREEN DIGITAL_PA3
#define POWER_BUTTON DIGITAL_PA5
#define POWER_ENABLE DIGITAL_PA7
#define MOSFET_CONTROL DIGITAL_PB8
#define SPI2_CS DIGITAL_PB12
#define DEBUG_LED DIGITAL_PC13

// CAN
#define CAN_MAP { &hcan, NULL, NULL }
#define CAN_0 0

// UART
#define UART_COUNT 1
#define UART_MAP { &huart1 }

#define DART_UART1 0
// Define debug UART
#define UART_LOG DART_UART1

// SPI
#define SPI_COUNT 1
#define SPI_MAP { &hspi2 }

#define DART_SPI 0

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c2 }

#define DART_I2C 0

#endif
#endif
