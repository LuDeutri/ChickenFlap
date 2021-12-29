/*
 * Configuration file for the AMS master 2019 r1 board.
 */

#ifdef USE_BOARD_AMS_2019_R1
#ifndef BOARD_AMS_2019_R1
#define BOARD_AMS_2019_R1

// DART_BOARD
#ifdef DART_BOARD
#error "Board already defined"
#endif
#define DART_BOARD

/**
 * Name of the board this configuration file defines.
 */
#define HAL_BOARD "AMS_2019_R1"

// Include any HAL definitions

// HAL config
#define USE_TMS570

//#define HAL_ENABLE_FREERTOS
#define HAL_ENABLE_TIME
#define HAL_ENABLE_CAN
#define HAL_ENABLE_ADC
#define HAL_ENABLE_GPIO
//#define HAL_ENABLE_UART
//#define HAL_ENABLE_I2C
#define HAL_ENABLE_SPI
//#define HAL_ENABLE_FILESYSTEM
//#define HAL_ENABLE_NETWORK
//#define HAL_ENABLE_HIGHRES_TIME
//#define HAL_ENABLE_RTC
//#define HAL_ENABLE_FLASH
#define HAL_ENABLE_PWM

//#define HAL_DUMMY_TIME
//#define HAL_DUMMY_CAN
//#define HAL_DUMMY_ADC
//#define HAL_DUMMY_GPIO
#define HAL_DUMMY_UART
//#define HAL_DUMMY_I2C
//#define HAL_DUMMY_SPI
//#define HAL_DUMMY_FILESYSTEM
//#define HAL_DUMMY_NETWORK
//#define HAL_DUMMY_HIGHRES_TIME
//#define HAL_DUMMY_RTC

//#define HAL_ENABLE_CAN_MCP2515
//#define HAL_ENABLE_NETWORK_LWIP
//#define HAL_ENABLE_FILESYSTEM_FATFS

// Analog

/**
 * Maximum value of a returned analog value. Values returned by getADC() are in the interval [0, ADC_MAX_VALUE].
 */
#define ADC_MAX_VALUE 4095

/**
 * Defines the count of ADC channels available.
 */
#define ADC_CHANNEL_COUNT 21

// Logical and physical name already match for TMS570, so no mapping is done
// Constants for mapping the physical name to logical ADC channel.
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3
#define ADC_CHANNEL_4 4
#define ADC_CHANNEL_5 5
#define ADC_CHANNEL_6 6
#define ADC_CHANNEL_7 7
#define ADC_CHANNEL_8 8
#define ADC_CHANNEL_9 9
#define ADC_CHANNEL_10 10
#define ADC_CHANNEL_11 11
#define ADC_CHANNEL_12 12
#define ADC_CHANNEL_13 13
#define ADC_CHANNEL_14 14
#define ADC_CHANNEL_15 15
#define ADC_CHANNEL_16 16
#define ADC_CHANNEL_17 17
#define ADC_CHANNEL_18 18
#define ADC_CHANNEL_19 19
#define ADC_CHANNEL_20 20

// TMS570 settings
#define ADC_TIMEOUT 5

// Enables the group 1
#define ADC_ENABLE_GROUP1
// Enables the group 2
//#define ADC_ENABLE_GROUP2

#define ADC_VREF_VOLTAGE 3.3f
#define ADC_BUFFER_VOLTAGE ADC_CHANNEL_2

// Digital
/**
 * Defines the count of digital channels available.
 */
#define DIGITAL_CHANNEL_COUNT 19

// TMS570 information: the adcREG1 register is handled differently and is NOT a gioPORT_t
// only cast to gioPORT_t to silence the warning

/**
 * Defines the mapping of the logical digital channels to the physical digital peripheral.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_PERIPHERAL {hetPORT1, hetPORT1, hetPORT1, hetPORT1, hetPORT1, hetPORT1, hetPORT1, (gioPORT_t*)adcREG1, gioPORTA, gioPORTA, gioPORTA, gioPORTA, gioPORTA, spiPORT2, spiPORT1, spiPORT3, linPORT, linPORT, spiPORT1}
/**
 * Defines the mapping of the logical digital channels to the physical digital channel.
 * Count of channels MUST be equal to DIGITAL_CHANNEL_COUNT.
 */
#define DIGITAL_MAP_CHANNEL {PIN_HET_0, PIN_HET_2, PIN_HET_4, PIN_HET_6, PIN_HET_17, PIN_HET_19, PIN_HET_24, ADC1_EVENT, 7, 0, 1, 2, 3, SPI_PIN_CS0, SPI_PIN_CS0, SPI_PIN_CS0, PIN_LIN_TX, PIN_LIN_RX, SPI_PIN_SOMI}

// Constants for mapping the physical name to logical digital channel.
#define DIGITAL_PIN_HET_0 0
#define DIGITAL_PIN_HET_2 1
#define DIGITAL_PIN_HET_4 2
#define DIGITAL_PIN_HET_6 3
#define DIGITAL_PIN_HET_17 4
#define DIGITAL_PIN_HET_19 5
#define DIGITAL_PIN_HET_24 6
#define DIGITAL_PIN_ADEVT 7
#define DIGITAL_PIN_GIOA7 8
#define DIGITAL_PIN_GIOA_0 9
#define DIGITAL_PIN_GIOA_1 10
#define DIGITAL_PIN_GIOA_2 11
#define DIGITAL_PIN_GIOA_3 12
#define DIGITAL_PIN_SPI2_CS0 13
#define DIGITAL_PIN_SPI1_CS0 14
#define DIGITAL_PIN_SPI3_CS0 15
#define DIGITAL_PIN_LIN_TX 16
#define DIGITAL_PIN_LIN_RX 17
#define DIGITAL_PIN_SPI1_MISO 18


// Define pins
#define DIGITAL_LED1 DIGITAL_PIN_ADEVT
#define DIGITAL_LED2 DIGITAL_PIN_HET_24
#define DIGITAL_K1_CTRL DIGITAL_PIN_HET_2
#define DIGITAL_K2_CTRL DIGITAL_PIN_HET_0
#define DIGITAL_K3_CTRL DIGITAL_PIN_GIOA7
#define DIGITAL_AMS_OUTPUT DIGITAL_PIN_HET_6
#define DIGITAL_AMS_HOLD DIGITAL_PIN_HET_4
#define DIGITAL_CS_EM_CURRENT DIGITAL_PIN_SPI1_CS0
#define DIGITAL_CS_EM_VOLTAGE DIGITAL_PIN_HET_17
#define DIGITAL_IMD_PWM DIGITAL_PIN_HET_19
#define DIGITAL_TIMER_250MS DIGITAL_PIN_SPI2_CS0
#define DIGITAL_VCU DIGITAL_PIN_GIOA_0
#define DIGITAL_IMD_HOLD DIGITAL_PIN_GIOA_1
#define DIGITAL_SDC_READY DIGITAL_PIN_GIOA_2
#define DIGITAL_RESET_BUTTON DIGITAL_PIN_GIOA_3
#define DIGITAL_HVDET_POSITIVE DIGITAL_PIN_LIN_RX
#define DIGITAL_HVDET_NEGATIVE DIGITAL_PIN_LIN_TX
#define DIGITAL_CS_ISOSPI DIGITAL_PIN_SPI3_CS0

// CAN
#define CAN_MAP { canREG1, canREG2, NULL }

// TMS570 settings
#define CAN_TIMEOUT 5

// Override CAN settings
#define CAN_MAX_RECEIVE_CALLBACKS 20
#define CAN_TRANSMIT_BUFFER_SIZE 30
// #define CAN_RECEIVE_BUFFER_SIZE 256
// #define CANDATA_ENABLE_FAST_PATH
#define CANDATA_RECEIVE_COUNT 16
#define CANDATA_TRANSMIT_COUNT 16

// UART
#define UART_COUNT 0

#define UART_MAP {}

// Define debug UART
// #define UART_LOG

// SPI
#define SPI_COUNT 2
#define SPI_MAP { spiREG1, spiREG3 }

#define SPI_BUS_1 0
#define SPI_BUS_3 1

// I2C
#define I2C_COUNT 0
#define I2C_MAP {}

// PWM
#define PWM_CHANNEL_COUNT 1
#define PWM_MAP { hetRAM1 }
#define PWM_CHANNEL_MAP { 6 }

#define PMW_N2HET1_CHANNEL6 0

#define PWM_FAN PMW_N2HET1_CHANNEL6


#endif
#endif
