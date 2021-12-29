#ifndef DART_DRIVER_LTC68XX_LTC68XX_BASE_H_
#define DART_DRIVER_LTC68XX_LTC68XX_BASE_H_


#include "../../../../chickenFlap/dart/digital.h"
#include "../../../../chickenFlap/dart/driver/pec15.h"
#include "../../../../chickenFlap/dart/spi.h"
#include "../../../../chickenFlap/dart/uart.h"
#include "../../../../chickenFlap/dart/util/endianness.h"
#include "../../../../chickenFlap/dart/util/timer.h"

#ifdef __cplusplus
extern "C" {
#endif

// Commands
#define LTC68XX_CMD_WRCFGA 1
#define LTC68XX_CMD_RDCFGA 2

#define LTC68XX_CMD_WRCFGB 36
#define LTC68XX_CMD_RDCFGB 38


#define LTC68XX_CMD_WRSCTRL 20
#define LTC68XX_CMD_RDSCTRL 22

#define LTC68XX_CMD_WRPWM 32
#define LTC68XX_CMD_RDPWM 34

#define LTC68XX_CMD_RDCVA 4
#define LTC68XX_CMD_RDCVB 6
#define LTC68XX_CMD_RDCVC 8
#define LTC68XX_CMD_RDCVD 10
#define LTC68XX_CMD_RDCVE 9
#define LTC68XX_CMD_RDCVF 11
#define LTC68XX_CMD_RDAUXA 12
#define LTC68XX_CMD_RDAUXB 14
#define LTC68XX_CMD_RDSTATA 16
#define LTC68XX_CMD_RDSTATB 18
#define LTC68XX_CMD_RDSID 44

#define LTC68XX_CMD_ADCV 608
#define LTC68XX_CMD_ADOW 552
#define LTC68XX_CMD_CVST 519
#define LTC68XX_CMD_ADAX 1120
#define LTC68XX_CMD_ADAXD 1024
#define LTC68XX_CMD_AXOW 1040
#define LTC68XX_CMD_AXST 1031
#define LTC68XX_CMD_ADSTAT 1128
#define LTC68XX_CMD_ADSTATD 1032
#define LTC68XX_CMD_STATST 1039
#define LTC68XX_CMD_ADCVAX 1135
#define LTC68XX_CMD_ADCVSC 1127
#define LTC68XX_CMD_CLRCELL 1809
#define LTC68XX_CMD_CLRAUX 1810
#define LTC68XX_CMD_CLRSTAT 1811
#define LTC68XX_CMD_PLADC 1812
#define LTC68XX_CMD_DIAGN 1813
#define LTC68XX_CMD_WRCOMM 1825
#define LTC68XX_CMD_RDCOMM 1826
#define LTC68XX_CMD_STCOMM 1827

// Constants

// Maximum number of cells the LTC68xx family supports
#define LTC68XX_MAX_CELL_COUNT 18

// Number of C pins of the LTC68xx family
#define LTC68XX_C_PIN_COUNT 19

// Number of bytes which are read or written in each command (register group)
#define LTC68XX_BYTES_PER_REGISTER_GROUP 6

// Number of commands written/read per COMM for I2C
#define LTC68XX_COMM_CMD_COUNT 3

// Cell channel selection for ADCV
#define LTC68XX_ADCV_ALL_CELLS 0

#define LTC68XX_ADC_MIN_VALUE 0x0000u
#define LTC68XX_ADC_MAX_VALUE 0xDFFFu
#define LTC68XX_ADC_FILTER_FAIL 0xFF00u

// These values are already correctly align to fit into the uint16_t register
#define LTC68XX_ICOMM_MASK                          0xF000
#define LTC68XX_FCOMM_MASK                          0x000F
// Write (W) commands
#define LTC68XX_ICOMM_I2C_W_START                   0x6000
#define LTC68XX_ICOMM_I2C_W_STOP                    0x1000
#define LTC68XX_ICOMM_I2C_W_BLANK                   0x0000
#define LTC68XX_ICOMM_I2C_W_NO_TRANSMIT             0x7000

#define LTC68XX_FCOMM_I2C_W_MASTER_ACK              0x0000
#define LTC68XX_FCOMM_I2C_W_MASTER_NACK             0x0008
#define LTC68XX_FCOMM_I2C_W_MASTER_NACK_STOP        0x0009

// Read (R) commands
#define LTC68XX_ICOMM_I2C_R_MASTER_START            0x6000
#define LTC68XX_ICOMM_I2C_R_MASTER_STOP             0x1000
#define LTC68XX_ICOMM_I2C_R_BLANK_SDA_LOW           0x0000
#define LTC68XX_ICOMM_I2C_R_BLANK_SDA_HIGH          0x7000

#define LTC68XX_FCOMM_I2C_R_MASTER_ACK              0x0000
#define LTC68XX_FCOMM_I2C_R_SLAVE_ACK               0x0007
#define LTC68XX_FCOMM_I2C_R_SLAVE_NACK              0x000F
#define LTC68XX_FCOMM_I2C_R_SLAVE_ACK_MASTER_STOP   0x0001
#define LTC68XX_FCOMM_I2C_R_SLAVE_NACK_MASTER_STOP  0x0009

// GPIOs
// GPIO4 and GPIO5 are used for I2C (on LTC6811)
#define LTC68XX_GPIO_1 0
#define LTC68XX_GPIO_2 1
#define LTC68XX_GPIO_3 2
#define LTC68XX_GPIO_4 3
#define LTC68XX_GPIO_5 4
#define LTC68XX_GPIO_6 5
#define LTC68XX_GPIO_7 6
#define LTC68XX_GPIO_8 7
#define LTC68XX_GPIO_9 8

typedef enum {
    LTC6804,
    LTC6810,
    LTC6811,
    LTC6813
} ltc68xx_device_type;

typedef enum {
    LTC68XX_ADC_26Hz,
    LTC68XX_ADC_422Hz,
    LTC68XX_ADC_1kHz,
    LTC68XX_ADC_2kHz,
    LTC68XX_ADC_3kHz,
    LTC68XX_ADC_7kHz,
    LTC68XX_ADC_14kHz,
    LTC68XX_ADC_27kHz
} ltc68xx_adc_mode_t;

typedef enum {
    DISCHARGE_WHILE_CONVERSION,
    DISALLOW_DISCHARGE,
    MUTE_DISCHARGE
} ltc68xx_discharge_conversion_mode_t;

typedef enum {
    LTC68XX_STATE_INIT,

    // Self testing
    LTC68XX_STATE_SELF_TEST_MULTIPLEXER,
    LTC68XX_STATE_SELF_TEST_CELL_VOLTAGES,
    LTC68XX_STATE_SELF_TEST_AUX_VOLTAGES,
    LTC68XX_STATE_SELF_TEST_STATUS_VOLTAGES,
    LTC68XX_STATE_SELF_TEST_OPEN_WIRE,

    // Normal operation
    LTC68XX_STATE_MEASURE_CELL_VOLTAGES,
    LTC68XX_STATE_MEASURE_AUX_VOLTAGES,
    LTC68XX_STATE_MEASURE_REDUNDANT_CELL_VOLTAGES,
    LTC68XX_STATE_MEASURE_STATUS_VOLTAGES
} ltc68xx_driver_state_t;

#pragma pack(push, 1)
typedef struct {
    // GPIOx Pin Control
    uint8_t GPIO5_to_1 : 5;
    // References Powered Up
    bool REFON : 1;
    // Discharge Timer Enable (READ ONLY)
    bool DTEN : 1;
    // ADC Mode Option Bit
    bool ADCOPT : 1;

    // Under voltage Comparison Voltage
    uint16_t VUV : 12;
    // Over voltage Comparison Voltage
    uint16_t VOV : 12;

    // Discharge cell control
    uint8_t DCC8_to_1;

    // Discharge timer
    uint8_t DCTO : 4;

    // Discharge cell control
    uint8_t DCC12_to_9 : 4;
} ltc68xx_config_a_t;

typedef struct {
    uint8_t DCC16_to_13 : 4;
    uint8_t GPIO9_to_6 : 4;
    bool MUTE : 1;
    bool FDRF : 1;
    uint8_t PS : 2;
    bool DTMEN : 1;
    bool DCC0 : 1;
    bool DCC18 : 1;
    bool DCC17 : 1;
    uint16_t : 16; // RSVD
    uint16_t : 16; // RSVD
} ltc68xx_config_b_t;

typedef struct {
    uint16_t G1V;
    uint16_t G2V;
    uint16_t G3V;
} ltc68xx_aux_a_t;

typedef struct {
    uint16_t G4V;
    uint16_t G5V;
    uint16_t REF;
} ltc68xx_aux_b_t;

typedef struct {
    // Sum of All Cells Measurement
    uint16_t SC;
    // Internal Die Temperature
    uint16_t ITMP;
    // Analog Power Supply Voltage
    uint16_t VA;
} ltc68xx_status_a_t;

typedef struct {
    // Digital Power Supply Voltage
    uint16_t VD;

    // Cell x Over-voltage/Under-voltage
    bool C4OV : 1;
    bool C4UV : 1;
    bool C3OV : 1;
    bool C3UV : 1;
    bool C2OV : 1;
    bool C2UV : 1;
    bool C1OV : 1;
    bool C1UV : 1;

    // Cell x Over-voltage/Under-voltage
    bool C8OV : 1;
    bool C8UV : 1;
    bool C7OV : 1;
    bool C7UV : 1;
    bool C6OV : 1;
    bool C6UV : 1;
    bool C5OV : 1;
    bool C5UV : 1;

    // Cell x Over-voltage/Under-voltage
    bool C12OV : 1;
    bool C12UV : 1;
    bool C11OV : 1;
    bool C11UV : 1;
    bool C10OV : 1;
    bool C10UV : 1;
    bool C9OV : 1;
    bool C9UV : 1;

    // Revision Code
    uint8_t REV : 4;
    uint8_t : 2;

    // Multiplexer Self Test Result
    bool MUXFAIL : 1;

    // Thermal Shutdown Status
    bool THSD : 1;
} ltc68xx_status_b_t;
#pragma pack(pop)

typedef struct {
    /**
     * Too many PEC errors occurred while reading data.
     */
    bool pec_error;
    /**
     * The self test for the config read back failed.
     */
    bool self_test_config_read_back_fail;
    /**
     * The self test for the multiplexer failed.
     */
    bool self_test_multiplexer_fail;
    /**
     * The self test for the cell voltages failed.
     */
    bool self_test_cell_voltages_fail;
    /**
     * The self test for the aux voltages failed.
     */
    bool self_test_aux_voltages_fail;
    /**
     * The self test for the status voltages failed.
     */
    bool self_test_status_fail;
    /**
     * The self test for the open wire failed.
     */
    bool self_test_open_wire_fail;
    /**
     * The LTC device has shutdown for thermal reasons.
     */
    bool thermal_shutdown;
    /**
     * The reference voltage of the LTC device is out of spec.
     */
    bool ref_voltage_wrong;
    /**
     * The analog power supply voltage of the LTC device is out of spec.
     */
    bool analog_power_supply_wrong;
    /**
     * The digital power supply voltage of the LTC device is out of spec.
     */
    bool digital_power_supply_wrong;
    /**
     * Any cell voltage is over or under the set limits.
     */
    bool cell_voltage_error;
    /**
     * Some measured cell voltage has an invalid ADC value.
     */
    bool cell_voltage_any_invalid;
    /**
     * Array which holds the status if any measured ADC voltage for the given channel is invald.
     */
    bool cell_voltage_invalid[LTC68XX_MAX_CELL_COUNT];
    /**
     * Open wire of cell pin detected.
     */
    bool open_wire_cell_pin[LTC68XX_C_PIN_COUNT];
} ltc68xx_error_t;

typedef struct {
    // Device type
    ltc68xx_device_type type;

    // Registers of device
    uint64_t sid;
    ltc68xx_config_a_t config_a;
    ltc68xx_config_a_t config_a_rx;
    ltc68xx_config_b_t config_b;
    ltc68xx_config_b_t config_b_rx;
    ltc68xx_status_a_t status_a;
    ltc68xx_status_b_t status_b;
    ltc68xx_aux_a_t aux_a;
    ltc68xx_aux_b_t aux_b;

    // I2C communication
    uint16_t commTx[LTC68XX_COMM_CMD_COUNT];
    uint16_t commRx[LTC68XX_COMM_CMD_COUNT];
    bool commNeedsAck[LTC68XX_COMM_CMD_COUNT];

    // Cell voltages
    uint16_t cellVoltage_a[3];
    uint16_t cellVoltage_b[3];
    uint16_t cellVoltage_c[3];
    uint16_t cellVoltage_d[3];
    uint16_t cellVoltage_e[3];
    uint16_t cellVoltage_f[3];

    // Open wire self test
    float openWireTestPuVoltages[LTC68XX_MAX_CELL_COUNT];
    float openWireTestPdVoltages[LTC68XX_MAX_CELL_COUNT];

    // Errors
	bool ignoreCellVoltageError; // CellVoltageError will be ignored if true
    ltc68xx_error_t error;
    int pec_error_counter;
} ltc68xx_t;

typedef struct {
    /**
     * The SPI bus the chain is attached to.
     */
    int bus;
    /**
     * The chip select (CS) pin the LTC IC or ISOSPI transceiver is attached to.
     */
    int cs;
    /**
     * The count of ICs in the daisy-chain of LTC ICs.
     */
    uint8_t numICs;

    /**
     * Array containing each device of the chain.
     */
    ltc68xx_t* devices;

    // Internal configuration
    bool useADAXD;
    bool useADSTATD;
    bool needLTC6813Registers;
    bool canUseSID;

    // State machine
    bool ready;
    int updateCounter;
    ltc68xx_driver_state_t state;
    bool firstStep;
    util_time_t stateTime;

    // Configuration
    bool skipSelfTest;
    util_time_t wakeupTime;
    ltc68xx_adc_mode_t adc_mode;
    ltc68xx_discharge_conversion_mode_t discharge_mode;
} ltc68xx_chain_t;


/**
 * Returns the maximum of GPIO this LTC device has.
 */
int ltc68xx_get_gpio_count(const ltc68xx_t device);

/**
 * Returns the maximum of cells this LTC device can measure.
 */
int ltc68xx_get_max_cell_count(const ltc68xx_t device);

/**
 * Returns the configuration value for the LTC68XX chip for the given ADC mode.
 */
uint8_t ltc68xx_convert_adc_mode(ltc68xx_adc_mode_t mode);

/**
 * Returns the time it takes to do one full ADC conversion in milliseconds.
 */
util_time_t ltc68xx_get_adc_time(ltc68xx_chain_t* chain);

#ifdef __cplusplus
}
#endif

#endif
