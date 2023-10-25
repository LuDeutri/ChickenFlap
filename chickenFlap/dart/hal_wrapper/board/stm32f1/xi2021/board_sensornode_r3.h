/**
 * Configuration file for the sensornode board revision 3.
 */
#ifdef USE_BOARD_SENSORNODE_R3
#ifndef BOARD_SENSORNODE_R3
#define BOARD_SENSORNODE_R3

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
#define HAL_BOARD "sensornode_r3"

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
#define HAL_DUMMY_HIGHRES_TIME


// Externs for the variables in main.c
extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
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
#define ADC_CHANNEL_COUNT 8

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP { &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1}

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8, ADC_CHANNEL_9 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_ADC1_IN0 0
#define ANALOG_ADC1_IN1 1
#define ANALOG_ADC2_IN4 2
#define ANALOG_ADC2_IN5 3
#define ANALOG_ADC2_IN6 4
#define ANALOG_ADC2_IN7 5
#define ANALOG_ADC2_IN8 6
#define ANALOG_ADC2_IN9 7

#define ANALOG_IN_PROT_7 ANALOG_ADC1_IN0
#define ANALOG_IN_PROT_3 ANALOG_ADC1_IN1
#define ANALOG_IN_PROT_6 ANALOG_ADC2_IN4
#define ANALOG_IN_PROT_2 ANALOG_ADC2_IN5
#define ANALOG_IN_PROT_5 ANALOG_ADC2_IN6
#define ANALOG_IN_PROT_1 ANALOG_ADC2_IN7
#define ANALOG_IN_PROT_4 ANALOG_ADC2_IN8
#define ANALOG_IN_PROT_0 ANALOG_ADC2_IN9

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_13CYCLES_5

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 11

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC, GPIOA /*Tim1*/, GPIOA /*Tim2*/, GPIOB /*Tim3*/, GPIOB /*Timer4*/ }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_5, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, /*timer*/ GPIO_PIN_10, GPIO_PIN_15, GPIO_PIN_4, GPIO_PIN_9 }

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PA8 0
#define DIGITAL_PA9 1
#define DIGITAL_PB5 2
#define DIGITAL_PB12 3
#define DIGITAL_PC13 4
#define DIGITAL_PC14 5
#define DIGITAL_PC15 6

//timer
#define DIGITAL_PA10 7
#define DIGITAL_PA15 8
#define DIGITAL_PB4 9
#define DIGITAL_PB9 10


#define DIGITAL_IN_PROT_1	DIGITAL_PA8
#define DIGITAL_IN_PROT_2 	DIGITAL_PA9
#define LIN_NSLP 			DIGITAL_PB5  // LIN NSLP
#define SPI2_CS 			DIGITAL_PB12 // SPI Chip Select
#define DBG_LED 			DIGITAL_PC13 // Debug LED
#define BRAKELIGHT_CTRL 	DIGITAL_PC14 // Brakelight BJET CTRL
#define DIGITAL_IN_PROT_3 	DIGITAL_PC15

//timer
#define DRAIN_1_ENABLE DIGITAL_PA10
#define DRAIN_2_ENABLE DIGITAL_PA15
#define DRAIN_3_ENABLE DIGITAL_PB4
#define DRAIN_4_ENABLE DIGITAL_PB9

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH

//DRAIN_STATUS pins are located at IO extender
/*
#define DRAIN_STATUS_PIN_1
#define DRAIN_STATUS_PIN_2
#define DRAIN_STATUS_PIN_3
#define DRAIN_STATUS_PIN_4
*/

/*
//TIMER Output
#define TIMER_CHANNEL_COUNT 1
#define TIMER_MAP { &htim1, &htim2, &htim3, &htim4 }
#define TIMER_MAX_PULSE {255,255,255,255}
#define TIMER_MAP_CHANNEL { TIM_CHANNEL_3, TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_4}

#define TIM1_CH3 0
#define TIM2_CH1 1
#define TIM3_CH1 2
#define TIM4_CH4 3

//define drain pwm pins
#define DRAIN_1_ENABLE TIM1_CH3
#define DRAIN_2_ENABLE TIM2_CH1
#define DRAIN_3_ENABLE TIM3_CH1
#define DRAIN_4_ENABLE TIM4_CH4
*/

// CAN
#define CAN_MAP { &hcan }
#define CAN_0 0

#define DART_CAN_1 CAN_0
#define CAN_TRANSMIT_INTERVAL 50

// UART
#define UART_COUNT 2
#define UART_MAP { &huart1, &huart2 }

#define LIN_PUMPS 0
#define RS232_TTL 1

#define UART_RECEIVE_BUFFER_SIZE 256

// SPI
#define SPI_COUNT 1
#define SPI_MAP { &hspi2 }

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c2 }
#define DART_I2C 0

#endif
#endif
