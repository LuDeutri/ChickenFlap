/*
 * Configuration file for the STM32F769I-DISCO board.
 */

#ifdef USE_BOARD_STM32F769I_DISCO
#ifndef BOARD_STM32F769I_DISCO
#define BOARD_STM32F769I_DISCO

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

#ifndef STM32F769xx
#error "No STM32F769xx target"
#endif


/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "STM32F69I_DISCO"

// Include any HAL definitions
#include <stm32f7xx_hal.h>
#include "../../../../../chickenFlap/dart/hal_wrapper/stm32f769i_disco/stm32f769i_discovery.h"
#include "../../../../../chickenFlap/dart/hal_wrapper/stm32f769i_disco/stm32f769i_discovery_lcd.h"
#include "../../../../../chickenFlap/dart/hal_wrapper/stm32f769i_disco/stm32f769i_discovery_ts.h"
#include "../../../../../chickenFlap/dart/can_baudrate.h"

// HAL config
#define USE_STM32
#define USE_STM32F7

#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
//#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
#define HAL_ENABLE_UART
//#define HAL_ENABLE_I2C
#define HAL_ENABLE_SPI
#define HAL_ENABLE_FILESYSTEM
#define HAL_ENABLE_NETWORK
#define HAL_ENABLE_HIGHRES_TIME

//#define HAL_DUMMY_TIME
//#define HAL_DUMMY_CAN
//#define HAL_DUMMY_ADC
//#define HAL_DUMMY_GPIO
//#define HAL_DUMMY_UART
#define HAL_DUMMY_I2C
//#define HAL_DUMMY_SPI
//#define HAL_DUMMY_FILESYSTEM
//#define HAL_DUMMY_NETWORK
//#define HAL_DUMMY_HIGHRES_TIME

#define HAL_ENABLE_CAN_MCP2515
#define HAL_ENABLE_NETWORK_LWIP
#define HAL_ENABLE_FILESYSTEM_FATFS

// Externs for the variables in main.c
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

extern TIM_HandleTypeDef htim7;

extern SPI_HandleTypeDef hspi2;

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 7

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP { &hadc1, &hadc1, &hadc1, &hadc1, &hadc3, &hadc3, &hadc3 }

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_16, ADC_CHANNEL_4, ADC_CHANNEL_6, ADC_CHANNEL_12, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_TEMPERATURE 0
#define ANALOG_ADC1_IN4 1
#define ANALOG_ADC1_IN6 2
#define ANALOG_ADC1_IN12 3
#define ANALOG_ADC3_IN6 4
#define ANALOG_ADC3_IN7 5
#define ANALOG_ADC3_IN8 6

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_15CYCLES

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 7

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOJ, GPIOJ, GPIOJ, GPIOJ, GPIOJ, GPIOJ, GPIOA }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_13, GPIO_PIN_0 }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PJ0 0
#define DIGITAL_PJ1 1
#define DIGITAL_PJ3 2
#define DIGITAL_PJ4 3
#define DIGITAL_PJ5 4
#define DIGITAL_PJ13 5
#define DIGITAL_PA0 6


#define DIGITAL_IO_SPEED GPIO_SPEED_HIGH

// Define debug pins
#define DIGITAL_LED_RED DIGITAL_PJ13
#define DIGITAL_LED_GREEN DIGITAL_PJ5
#define DIGITAL_BUTTON DIGITAL_PA0

// CAN
#define CAN_MAP { NULL, NULL, NULL }

// UART
#define UART_COUNT 3

#define UART_MAP { &huart1, &huart5, &huart6 }

#define DART_UART1 0
#define DART_UART5 1
#define DART_UART6 2

// Define debug UART
#define UART_LOG DART_UART1

// SPI
#define SPI_COUNT 1
#define SPI_MAP { &hspi2 }

#define SPI_BUS2 0

// I2C
#define I2C_COUNT 0
#define I2C_MAP { }

// MCP2515

#define MCP2515_SPI_MAP { SPI_BUS2, SPI_BUS2, SPI_BUS2 }
#define MCP2515_PIN_CS_MAP { DIGITAL_PJ4, DIGITAL_PJ3, DIGITAL_PJ1 }
#define MCP2515_CAN_BAUDRATE { CAN_RATE_1000KBPS, CAN_RATE_500KBPS, CAN_RATE_500KBPS }

// High Resolution Timer
#define HIGHRES_TIMER &htim7
#define HIGHRES_TIMER_MAX_VALUE 0xFFFFu
#define HIGHRES_TIMER_FACTOR 1
#endif
#endif
