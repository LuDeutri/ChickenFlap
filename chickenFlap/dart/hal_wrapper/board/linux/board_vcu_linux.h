/*
 * Configuration file for the Windows board.
 */

#ifdef USE_BOARD_VCU_LINUX
#ifndef BOARD_LINUX
#define BOARD_LINUX

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

#if !defined(__linux)
#error "No Linux target"
#endif


/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "VCU_LINUX"

// Include any HAL definitions
#include "../../../../../chickenFlap/dart/hal_wrapper/linux/linux_includes.h"

// HAL config
#define USE_LINUX

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
//#define HAL_ENABLE_CAN
//#define HAL_ENABLE_ADC
//#define HAL_ENABLE_GPIO
#define HAL_ENABLE_UART
//#define HAL_ENABLE_I2C
//#define HAL_ENABLE_SPI
#define HAL_ENABLE_SDCARD
#define HAL_ENABLE_ETHERNET
#define HAL_ENABLE_HIGHRES_TIME
//#define HAL_ENABLE_RTC

//#define HAL_DUMMY_TIME
#define HAL_DUMMY_CAN
#define HAL_DUMMY_ADC
#define HAL_DUMMY_GPIO
//#define HAL_DUMMY_UART
#define HAL_DUMMY_I2C
#define HAL_DUMMY_SPI
//#define HAL_DUMMY_SDCARD
//#define HAL_DUMMY_ETHERNET
//#define HAL_DUMMY_HIGHRES_TIME
#define HAL_DUMMY_RTC
#define HAL_DUMMY_FLASH
#define HAL_DUMMY_PWM
#define HAL_DUMMY_DAC

#define HAL_ENABLE_TIME_SCALE

// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

 /**
  * Defines the count of ADC channels available.
  */
#define ADC_CHANNEL_COUNT 15

  /**
   * Defines the mapping of the logical ADC channels to the physical ADCs.
   * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
   */
#define ADC_MAP { &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc3, &hadc3, &hadc3, &hadc3, &hadc3, &hadc1 }

   /**
	* Defines the mapping of the logical ADC channels to the physical ADC channels.
	* Count of channels MUST be equal to ADC_CHANNEL_COUNT.
	*/
#define ADC_MAP_CHANNELS { ADC_CHANNEL_18, ADC_CHANNEL_0, ADC_CHANNEL_3, ADC_CHANNEL_6, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_10, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_5 }

	// Constants for mapping the physical name to logical ADC channel.
#define ANALOG_VBAT 0
#define ANALOG_ADC1_IN0 1
#define ANALOG_ADC1_IN3 2
#define ANALOG_ADC1_IN6 3
#define ANALOG_ADC1_IN8 4
#define ANALOG_ADC1_IN9 5
#define ANALOG_ADC1_IN10 6
#define ANALOG_ADC1_IN12 7
#define ANALOG_ADC1_IN13 8
#define ANALOG_ADC3_IN5 9
#define ANALOG_ADC3_IN6 10
#define ANALOG_ADC3_IN7 11
#define ANALOG_ADC3_IN8 12
#define ANALOG_ADC3_IN9 13
#define ANALOG_ADC1_IN5 14

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_15CYCLES

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 30

 /**
  * Defines the mapping of the logical digital channels to the physical digital peripheral.
  * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
  */
#define DIGITAL_MAP_PERIPHERAL { GPIOB, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOE, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOF, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOG, GPIOE, GPIOE, GPIOB, GPIOB }

  /**
   * Defines the mapping of the logical digital channels to the physical digital channel.
   * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
   */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_2, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_15, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_8, GPIO_PIN_9 }

   // Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PB2 0
#define DIGITAL_PD8 1
#define DIGITAL_PD9 2
#define DIGITAL_PD10 3
#define DIGITAL_PD11 4
#define DIGITAL_PD12 5
#define DIGITAL_PD13 6
#define DIGITAL_PD15 7
#define DIGITAL_PE15 8
#define DIGITAL_PF0 9
#define DIGITAL_PF1 10
#define DIGITAL_PF2 11
#define DIGITAL_PF11 12
#define DIGITAL_PF12 13
#define DIGITAL_PF13 14
#define DIGITAL_PG0 15
#define DIGITAL_PG2 16
#define DIGITAL_PG3 17
#define DIGITAL_PG4 18
#define DIGITAL_PG5 19
#define DIGITAL_PG6 20
#define DIGITAL_PG7 21
#define DIGITAL_PG10 22
#define DIGITAL_PG12 23
#define DIGITAL_PG13 24
#define DIGITAL_PG14 25
#define DIGITAL_PE10 26
#define DIGITAL_PE11 27
#define DIGITAL_PB8 28
#define DIGITAL_PB9 29

#define DIGITAL_IO_SPEED GPIO_SPEED_HIGH

// Define debug pins
#define DIGITAL_LED_RED DIGITAL_PG3
#define DIGITAL_LED_ORANGE DIGITAL_PG4
#define DIGITAL_LED_YELLOW DIGITAL_PG5
#define DIGITAL_LED_GREEN DIGITAL_PG6
#define DIGITAL_LED_BLUE DIGITAL_PG7

// CAN
#define CAN_MAP { NULL, NULL, NULL }

// Override can settings
#define CAN_MAX_RECEIVE_CALLBACKS 128
#define CAN_TRANSMIT_BUFFER_SIZE 128
#define CAN_RECEIVE_BUFFER_SIZE 256
#define CANDATA_RECEIVE_COUNT 128
#define CANDATA_TRANSMIT_COUNT 128
#define CANDATA_ENABLE_FAST_PATH

// UART
#define UART_COUNT 4
#define DART_UART1 0
#define DART_UART4 1
#define DART_UART7 2
#define DART_UART8 3

// Define debug UART
#define UART_LOG DART_UART1

// SPI
#define SPI_COUNT 3
#define SPI_MAP { NULL, NULL, NULL }

#define SPI_BUS1 0
#define SPI_BUS2 1
#define SPI_BUS4 2

// PWM
#define TIM3_CH1 0
#define TIM3_CH2 1

// I2C
#define I2C_COUNT 1
#define I2C_MAP { NULL }
#define I2C_BUS4 0

// Display
#define USE_DUMMY_DISPLAY

#define FLASH_MEMORY_SECTOR 0
#define FLASH_MEMORY_POINTER NULL


#endif
#endif
