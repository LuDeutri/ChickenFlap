/**
 * Configuration file for the chickenFlap revision 1
 */
#ifdef USE_BOARD_CHICKENFLAP_R2
#ifndef BOARD_CHICKENFLAP_R2
#define BOARD_CHICKENFLAP_R2

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

#ifndef STM32F745xx
#error "No STM32F745xx target"
#endif

/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "chickenFlap_r2"

// Include any HAL definitions
#include "stm32f7xx_hal.h"

// HAL config
#define USE_STM32
#define USE_STM32F7

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
//#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
#define HAL_ENABLE_UART
#define HAL_ENABLE_I2C
//#define HAL_ENABLE_SPI
//#define HAL_ENABLE_FILESYSTEM
//#define HAL_ENABLE_NETWORK
//#define HAL_ENABLE_HIGHRES_TIME
#define HAL_ENABLE_RTC
//#define HAL_ENABLE_FLASH
#define HAL_ENABLE_PWM
//#define HAL_ENABLE_DAC

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
//#define HAL_DUMMY_PWM


// Externs for the variables in main.c
extern ADC_HandleTypeDef hadc1;
//extern CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c4;
//extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim1;
extern RTC_HandleTypeDef hrtc;
// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095
#define ADC_MIN_VALUE 0
#define ADC_VREF 2049

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 2

/**
 * Defines the mapping of the logical ADC channels to the physical ADCs.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP { &hadc1, &hadc1 }

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_8, ADC_CHANNEL_9 }

// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_ADC1_IN8 0
#define ANALOG_ADC1_IN9 1

#define ADC_BATTERY_VOLTAGE ANALOG_ADC1_IN8
#define ADC_MOTORSPEED 		ANALOG_ADC1_IN9

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_15CYCLES

// Digital
#define DIGITAL_CHANNEL_COUNT 20

#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOD, GPIOD, GPIOE, GPIOE, GPIOF, GPIOF, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG }

#define DIGITAL_MAP_CHANNEL { GPIO_PIN_2, GPIO_PIN_9, GPIO_PIN_12, GPIO_PIN_15, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_10, GPIO_PIN_7, GPIO_PIN_10, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13 }

#define DIGITAL_PA2 0
#define DIGITAL_PA9 1
#define DIGITAL_PA12 2
#define DIGITAL_PA15 3
#define DIGITAL_PB12 4
#define DIGITAL_PB13 5
#define DIGITAL_PB14 6
#define DIGITAL_PB15 7
#define DIGITAL_PC10 8
#define DIGITAL_PD7 9
#define DIGITAL_PD10 10
#define DIGITAL_PE2 11
#define DIGITAL_PE3 12
#define DIGITAL_PF9 13
#define DIGITAL_PF8 14
#define DIGITAL_PG9 15
#define DIGITAL_PG10 16
#define DIGITAL_PG11 17
#define DIGITAL_PG12 18
#define DIGITAL_PG13 19

// Buttons
#define BUTTON_FLAP_CTRL 	DIGITAL_PE2
#define BUTTON_MENU_ENTER 	DIGITAL_PE3
#define BUTTON_RIGHT		DIGITAL_PF9
#define BUTTON_LEFT			DIGITAL_PB12
#define BUTTON_MENU_BACK	DIGITAL_PC10
#define	BUTTON_JOKER		DIGITAL_PA15

// Motor
#define MOTOR_ENABLE_FLAP_OPEN		DIGITAL_PA9
#define MOTOR_ENABLE_FLAP_CLOSE		DIGITAL_PA12
#define MOTOR_VCC_CTRL 				DIGITAL_PB13

// Modules
#define MODULE_DIODE			DIGITAL_PB14
#define MODULE_PHOTORESISTOR 	DIGITAL_PB15
#define ESP_RST					DIGITAL_PA2
#define DISPLAY_RST 			DIGITAL_PD10
#define AUDIO_STATUS			DIGITAL_PF8

// LEDs
#define ERROR_LED			DIGITAL_PD7
#define	DEBUG_LED_BLUE		DIGITAL_PG9
#define	DEBUG_LED_GREEN		DIGITAL_PG10
#define	DEBUG_LED_YELLOW	DIGITAL_PG11
#define	DEBUG_LED_ORANGE	DIGITAL_PG12
#define	DEBUG_LED_RED		DIGITAL_PG13

#define DIGITAL_IO_SPEED GPIO_SPEED_FREQ_HIGH



// Timer
#define TIMER_CHANNEL_COUNT 1
#define TIMER_MAP { &htim1 }
#define TIMER_MAX_PULSE {255}
#define TIMER_MAP_CHANNEL { TIM_CHANNEL_3 }

#define TIM1_CH3 0

//define drain pwm pins
#define MOTOR_DUTY_CYCLE TIM1_CH3

#define MAX_TIMER_PWM 65000



// UART
#define UART_COUNT 3
#define UART_MAP { &huart4, &huart7, &huart3 }

//#define UART4 0
//#define UART7 1
//#define USART3 2

#define UART_ESP UART4
#define UART_AUDIO UART7
#define UART_DEBUG USART3

// SPI
#define SPI_COUNT 0

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c4 }
#define I2C_BUS 0

#endif
#endif
