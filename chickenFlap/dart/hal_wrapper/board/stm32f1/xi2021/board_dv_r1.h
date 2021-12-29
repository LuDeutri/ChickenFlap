/**
 * Configuration file for the EBS logic.
 */

#ifdef USE_BOARD_DV_R1
#ifndef BOARD_DV_R1
#define BOARD_DV_R1

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
#define HAL_BOARD "STM32F103_DV_R1"

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
extern ADC_HandleTypeDef hadc1;

// Analog
/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 5

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP { &hadc1, &hadc1, &hadc1, &hadc1, &hadc1 }

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_16, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_TEMPERATURE 0
#define ANALOG_ADC1_IN1 1
#define ANALOG_ADC1_IN2 2
#define ANALOG_ADC1_IN3 3
#define ANALOG_ADC1_IN4 4

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_13CYCLES_5

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 22

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOC, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, GPIOA, GPIOB  }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_13, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_10, GPIO_PIN_15, GPIO_PIN_13 }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PC13 0
#define DIGITAL_PA5 1
#define DIGITAL_PA6 2
#define DIGITAL_PA7 3
#define DIGITAL_PA8 4
#define DIGITAL_PA9 5
#define DIGITAL_PB0 6
#define DIGITAL_PB1 7
#define DIGITAL_PB2 8
#define DIGITAL_PB3 9
#define DIGITAL_PB4 10
#define DIGITAL_PB5 11
#define DIGITAL_PB8 12
#define DIGITAL_PB9 13
#define DIGITAL_PB10 14
#define DIGITAL_PB11 15
#define DIGITAL_PB12 16
#define DIGITAL_PB14 17
#define DIGITAL_PB15 18
#define DIGITAL_PA10 19
#define DIGITAL_PA15 20
#define DIGITAL_PB13 21

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

// Define debug pins
#define DIGITAL_LED_DEBUG DIGITAL_PC13

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
