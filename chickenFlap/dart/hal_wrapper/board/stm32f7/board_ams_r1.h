/*
 * Configuration file for the Accumulator Managment System r1 board.
 */

#ifdef USE_BOARD_AMS_R1
#ifndef BOARD_AMS_R1
#define BOARD_AMS_R1

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

#ifndef STM32F732xx
#error "No STM32F732xx target"
#endif


/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "AMS_R1"

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
//#define HAL_ENABLE_I2C
#define HAL_ENABLE_SPI
//#define HAL_ENABLE_FILESYSTEM
//#define HAL_ENABLE_NETWORK
//#define HAL_ENABLE_HIGHRES_TIME
//#define HAL_ENABLE_RTC
#define HAL_ENABLE_FLASH

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

//#define HAL_ENABLE_CAN_MCP2515
//#define HAL_ENABLE_NETWORK_LWIP
//#define HAL_ENABLE_FILESYSTEM_FATFS

// Externs for the variables in main.c
extern ADC_HandleTypeDef hadc1;

extern CAN_HandleTypeDef hcan1;

extern DMA_HandleTypeDef hdma_sdmmc1_rx;
extern DMA_HandleTypeDef hdma_sdmmc1_tx;

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;

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
#define cont_meas 0

#define ANALOG_SAMPLE_TIME ADC_SAMPLETIME_15CYCLES

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 17

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL { GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC }

/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_12, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7 }
// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PA0 0
#define DIGITAL_PA1 1
#define DIGITAL_PA2 2
#define DIGITAL_PA3 3
#define DIGITAL_PA6 4
#define DIGITAL_PB0 5
#define DIGITAL_PB2 6
#define DIGITAL_PB4 7
#define DIGITAL_PB6 8
#define DIGITAL_PB7 9
#define DIGITAL_PB12 9
#define DIGITAL_PC1 10
#define DIGITAL_PC2 11
#define DIGITAL_PC3 12
#define DIGITAL_PC4 13
#define DIGITAL_PC6 14
#define DIGITAL_PC7 15

#define DIGITAL_IO_SPEED GPIO_SPEED_HIGH

// Define debug pins
#define HVDET_MINUS DIGITAL_PA0
#define HVDET_PLUS DIGITAL_PA1
#define WDI DIGITAL_PA2
#define RESET DIGITAL_PA3
#define SDC_DELAY DIGITAL_PA6
#define AMS DIGITAL_PB0
#define AMS_HOLD DIGITAL_PB2
#define SDCARD_CD DIGITAL_PB4
#define SPI3_CS2 DIGITAL_PB6
#define SPI3_CS1 DIGITAL_PB7
#define SPI2_CS DIGITAL_PB12
#define SDC_CLOSED DIGITAL_PC1
#define LED_1 DIGITAL_PC2
#define LED_2 DIGITAL_PC3
#define IMD_HOLD DIGITAL_PC4
#define AIR_PLUS DIGITAL_PC6
#define AIR_MINUS DIGITAL_PC7

// CAN
#define CAN_MAP { &hcan1}

// Override CAN settings
//#define CAN_MAX_RECEIVE_CALLBACKS 128
//#define CAN_TRANSMIT_BUFFER_SIZE 128
//#define CAN_RECEIVE_BUFFER_SIZE 256
//#define CANDATA_ENABLE_FAST_PATH

// UART
#define UART_COUNT 1

#define UART_MAP { &huart1 }

#define DART_UART1 0

// SPI
#define SPI_COUNT 2
#define SPI_MAP { &hspi2, &hspi3 }

#define SPI_BUS2 0
#define SPI_BUS3 1

// I2C
#define I2C_COUNT 0

// TIMER
#define TIMER_CHANNEL_COUNT 5
#define TIMER_MAP { &htim2, &htim2, &htim2, &htim3, &htim14 }
#define TIMER_MAX_PULSE { 255, 255, 255, 255, 255 }
#define TIMER_MAP_CHANNEL { TIM_CHANNEL_1, TIM_CHANNEL_3, TIM_CHANNEL_4, TIM_CHANNEL_4, TIM_CHANNEL_1 }

#define FAN_PWM 0
#define AIR_MINUS_CTRL 1
#define PRECHARGE_CTRL 2
#define AIR_PLUS_CTRL 3
#define IMD_PWM 4

// Flash memory
extern __attribute__((__section__(".settings_data"))) uint8_t flashMemory[256 * 1024];

#define FLASH_MEMORY_SECTOR FLASH_SECTOR_11
#define FLASH_MEMORY_POINTER flashMemory


#endif
#endif
