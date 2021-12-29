/**
 * Configuration file for the chickenFlap revision 1
 */
#ifdef USE_BOARD_CHICKENFLAP_R1
#ifndef BOARD_CHICKENFLAP_R1
#define BOARD_CHICKENFLAP_R1

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
#define HAL_BOARD "chickenFlap_r1"

// Include any HAL definitions
#include <stm32f1xx_hal.h>

// HAL config
#define USE_STM32
#define USE_STM32F1

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
//#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
//#define HAL_ENABLE_UART
#define HAL_ENABLE_I2C
//#define HAL_ENABLE_SPI
//#define HAL_ENABLE_PWM
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
//extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c1;
//extern SPI_HandleTypeDef hspi2;
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
//extern TIM_HandleTypeDef htim1;
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim3;
//extern TIM_HandleTypeDef htim4;
// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095
#define ADC_MIN_VALUE 0

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
#define ADC_MAP_CHANNELS { ADC_CHANNEL_0 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_ADC1_IN0 0

#define ADC_BATTERY_VOLTAGE ANALOG_ADC1_IN0

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_13CYCLES_5

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 10

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOC, GPIOC }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_13, GPIO_PIN_14 }

#define DIGITAL_PA1 0
#define DIGITAL_PA2 1
#define DIGITAL_PA3 2
#define DIGITAL_PA4 3
#define DIGITAL_PA5 4
#define DIGITAL_PA6 5
#define DIGITAL_PA7 6
#define DIGITAL_PB0 7
#define DIGITAL_PC13 8
#define DIGITAL_PC14 9

#define MOTOR_FLAP_CLOSE 	DIGITAL_PA1
#define MOTOR_FLAP_OPEN 	DIGITAL_PA2
#define BUTTON_MENU_BACK 	DIGITAL_PA3
#define BUTTON_MENU_ENTER	DIGITAL_PA4
#define BUTTON_LEFT 		DIGITAL_PA5
#define BUTTON_RIGHT 		DIGITAL_PA6
#define BUTTON_FLAP_CTRL 	DIGITAL_PA7
#define DISPLAY_ENABLE		DIGITAL_PB0
#define DEBUG_LED 			DIGITAL_PC13
#define ERROR_LED			DIGITAL_PC14

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

// UART
#define UART_COUNT 0

// SPI
#define SPI_COUNT 0

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c1 }
#define I2C 0

#endif
#endif
