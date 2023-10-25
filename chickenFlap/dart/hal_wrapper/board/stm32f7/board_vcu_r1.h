/*
 * Configuration file for the Vehicle Control Unit r1 board.
 */

#ifdef USE_BOARD_VCU_R1
#ifndef BOARD_VCU_R1
#define BOARD_VCU_R1

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

#ifndef STM32F765xx
#error "No STM32F765xx target"
#endif


/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "VCU_R1"

// Include any HAL definitions
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_flash.h"
#include "stm32f7xx_hal_flash_ex.h"
#include "../../../../../chickenFlap/dart/can_baudrate.h"

// HAL config
#define USE_STM32
#define USE_STM32F7

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
#define HAL_ENABLE_UART
#define HAL_ENABLE_I2C
#define HAL_ENABLE_SPI
#define HAL_ENABLE_FILESYSTEM
#define HAL_ENABLE_NETWORK
#define HAL_ENABLE_HIGHRES_TIME
#define HAL_ENABLE_RTC
#define HAL_ENABLE_FLASH
#define HAL_ENABLE_PWM
#define HAL_ENABLE_DAC

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

//#define HAL_ENABLE_CAN_MCP2515
#define HAL_ENABLE_NETWORK_LWIP
#define HAL_ENABLE_FILESYSTEM_FATFS

// Externs for the variables in main.c
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern CAN_HandleTypeDef hcan3;

extern DAC_HandleTypeDef hdac;

extern I2C_HandleTypeDef hi2c4;

extern RNG_HandleTypeDef hrng;

extern RTC_HandleTypeDef hrtc;

extern SD_HandleTypeDef hsd1;
extern DMA_HandleTypeDef hdma_sdmmc1_rx;
extern DMA_HandleTypeDef hdma_sdmmc1_tx;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi4;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart2;


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
#define ADC_MAP { &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc1, &hadc3, &hadc3, &hadc3, &hadc3, &hadc3 }

/**
 * Defines the mapping of the logical ADC channels to the physical ADC channels.
 * Count of channels MUST be equal to ADC_CHANNEL_COUNT.
 */
#define ADC_MAP_CHANNELS { ADC_CHANNEL_18, ADC_CHANNEL_0, ADC_CHANNEL_3, ADC_CHANNEL_6, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_10, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_5, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8, ADC_CHANNEL_9 }

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
#define ANALOG_ADC1_IN5 9
#define ANALOG_ADC3_IN5 10
#define ANALOG_ADC3_IN6 11
#define ANALOG_ADC3_IN7 12
#define ANALOG_ADC3_IN8 13
#define ANALOG_ADC3_IN9 14

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_15CYCLES


// DAC
/**
 * Defines the count of DAC channels available.
 */
#define DAC_CHANNEL_COUNT 1

/**
 * Defines the mapping of the logical DAC channels to the physical DAC peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DAC_MAP_PERIPHERAL { &hdac }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DAC_MAP_CHANNEL { DAC_CHANNEL_1 }



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
#define CAN_MAP { &hcan1, &hcan2, &hcan3 }

// Override CAN settings
#define CAN_MAX_RECEIVE_CALLBACKS 128
#define CAN_TRANSMIT_BUFFER_SIZE 128
#define CAN_RECEIVE_BUFFER_SIZE 256
#define CANDATA_RECEIVE_COUNT 128
#define CANDATA_TRANSMIT_COUNT 128
#define CANDATA_ENABLE_FAST_PATH

// UART
#define UART_COUNT 4

#define UART_MAP { &huart2, &huart4, &huart7, &huart8 }

#define DART_UART2 0
#define DART_UART4 1
#define DART_UART7 2
#define DART_UART8 3

#define UART_RECEIVE_BUFFER_SIZE 4096

// Define debug UART
#define UART_LOG DART_UART8

// SPI
#define SPI_COUNT 3
#define SPI_MAP { &hspi1, &hspi2, &hspi4 }

#define SPI_BUS1 0
#define SPI_BUS2 1
#define SPI_BUS4 2

// I2C
#define I2C_COUNT 1
#define I2C_MAP { &hi2c4 }
#define I2C_BUS4 0

// High Resolution Timer
#define HIGHRES_TIMER &htim7
#define HIGHRES_TIMER_MAX_VALUE 0xFFFFu
#define HIGHRES_TIMER_FACTOR 1

// PWM
#define TIMER_CHANNEL_COUNT 2
#define TIMER_MAP { &htim3, &htim3 }
#define TIMER_MAX_PULSE { 255, 255 }
#define TIMER_MAP_CHANNEL { TIM_CHANNEL_1, TIM_CHANNEL_2 }

#define TIM3_CH1 0
#define TIM3_CH2 1

// Display
#define USE_EVE2_DISPLAY

// Flash memory
extern __attribute__((__section__(".settings_data"))) uint8_t flashMemory[256 * 1024];

#define FLASH_MEMORY_SECTOR FLASH_SECTOR_11
#define FLASH_MEMORY_POINTER flashMemory


#endif
#endif
