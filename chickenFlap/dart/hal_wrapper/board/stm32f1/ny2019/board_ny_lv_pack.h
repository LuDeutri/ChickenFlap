/**
 * Configuration file for the Status Indicator.
 */

#ifdef USE_BOARD_NY_LV_PACK
#ifndef BOARD_NY_LV_PACK
#define BOARD_NY_LV_PACK

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
#define HAL_BOARD "STM32F103_NY_LV_PACK"

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
extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 4

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP { &hadc1, &hadc1, &hadc1, &hadc1 }

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_16, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_TEMPERATURE 0
#define ANALOG_ADC1_IN2 1
#define ANALOG_ADC1_IN3 2
#define ANALOG_ADC1_IN4 3

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_13CYCLES_5

// ADC channels for the temp sensors
#define ADC_TEMP_1 ANALOG_ADC1_IN2
#define ADC_TEMP_2 ANALOG_ADC1_IN3
#define ADC_TEMP_3 ANALOG_ADC1_IN4

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 5

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOA, GPIOA, GPIOA }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_1, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8}

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PA1 0
#define DIGITAL_PA5 1
#define DIGITAL_PA6 2
#define DIGITAL_PA7 3
#define DIGITAL_PA8 4

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

// Define debug pins
#define DIGITAL_LED_DEBUG DIGITAL_PA5
#define DIGITAL_LED_ERROR DIGITAL_PA1
#define DIGITAL_CTRL DIGITAL_PA8
#define DIGITAL_RESET_BTN DIGITAL_PA6
#define DIGITAL_SHUTDOWN_BTN DIGITAL_PA7

// CAN
#define CAN_MAP { &hcan, NULL, NULL }
#define CAN_1 0

#define CAN_MAX_RECEIVE_CALLBACKS 4
#define CAN_TRANSMIT_BUFFER_SIZE 16
#define CAN_RECEIVE_BUFFER_SIZE 32
// #define CANDATA_ENABLE_FAST_PATH
#define CANDATA_RECEIVE_COUNT 8
#define CANDATA_TRANSMIT_COUNT 8
#define CAN_SEND_USE_LAX_DLC_CHECK

// UART
#define UART_COUNT 1
#define UART_MAP { &huart1 }

#define UART_LOG 0

// SPI
#define SPI_COUNT 0
#define SPI_MAP {}

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c2 }

#define I2C_BUS_2 0

#endif
#endif
