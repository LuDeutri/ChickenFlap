#ifndef DART_DRIVER_LTC6810_H_
#define DART_DRIVER_LTC6810_H_

#include "../../../chickenFlap/dart/digital.h"
#include "../../../chickenFlap/dart/driver/pec15.h"
#include "../../../chickenFlap/dart/spi.h"
#include "../../../chickenFlap/dart/uart.h"
#include "../../../chickenFlap/dart/util/timer.h"

#ifdef __cplusplus
extern "C" {
#endif

// Commands
#define LTC6810_CMD_WRCFG 1
#define LTC6810_CMD_RDCFG 2

#define LTC6810_CMD_WRSCTRL 20
#define LTC6810_CMD_RDSCTRL 22

#define LTC6810_CMD_WRPWM 32
#define LTC6810_CMD_RDPWM 34

#define LTC6810_CMD_RDCVA 4
#define LTC6810_CMD_RDCVB 6
#define LTC6810_CMD_RDSA 8
#define LTC6810_CMD_RDSB 10
#define LTC6810_CMD_RDAUXA 12
#define LTC6810_CMD_RDAUXB 14
#define LTC6810_CMD_RDSTATA 16
#define LTC6810_CMD_RDSTATB 18
#define LTC6810_CMD_RDSID 44

#define LTC6810_CMD_ADCV 608
#define LTC6810_CMD_ADOW 552
#define LTC6810_CMD_CVST 519
#define LTC6810_CMD_ADAX 1120
#define LTC6810_CMD_ADAXD 1024
#define LTC6810_CMD_AXOW 1040
#define LTC6810_CMD_AXST 1031
#define LTC6810_CMD_ADSTAT 1128
#define LTC6810_CMD_ADSTATD 1032
#define LTC6810_CMD_STATST 1039
#define LTC6810_CMD_ADCVAX 1135
#define LTC6810_CMD_ADCVSC 1127
#define LTC6810_CMD_CLRCELL 1809
#define LTC6810_CMD_CLRAUX 1810
#define LTC6810_CMD_CLRSTAT 1811
#define LTC6810_CMD_PLADC 1812
#define LTC6810_CMD_DIAGN 1813
#define LTC6810_CMD_WRCOMM 1825
#define LTC6810_CMD_RDCOMM 1826
#define LTC6810_CMD_STCOMM 1827
#define LTC6810_CMD_MUTE 40
#define LTC6810_CMD_UNMUTE 41

// Constants
#define LTC6810_CELL_COUNT 6
#define LTC6810_C_PIN_COUNT 7

#define LTC6810_BYTES_PER_REGISTER_GROUP 6

#define LTC6810_ADCV_ALL_CELLS 0

#define LTC6810_ADC_MIN_VALUE 0x0000u
#define LTC6810_ADC_MAX_VALUE 0xDFFFu
#define LTC6810_ADC_FILTER_FAIL 0xFF00u

#define LTC6810_COMM_I2CSTART     0x6000
#define LTC6810_COMM_I2CSTOP      0x1000
#define LTC6810_COMM_I2CNOP       0x7000
#define LTC6810_COMM_I2CNACK      0x0008
#define LTC6810_COMM_I2CNACKSTOP  0x0009
#define LTC6810_COMM_I2CWRITE     0x0000
#define LTC6810_COMM_I2CREAD      0x0010


typedef enum {
	LTC6810_ADC_26Hz,
	LTC6810_ADC_422Hz,
	LTC6810_ADC_1kHz,
	LTC6810_ADC_2kHz,
	LTC6810_ADC_3kHz,
	LTC6810_ADC_7kHz,
	LTC6810_ADC_14kHz,
	LTC6810_ADC_27kHz
} ltc6810_adc_mode_t;

typedef enum {
	DISCHARGE_WHILE_CONVERSION,
	DISALLOW_DISCHARGE,
	MUTE_DISCHARGE
} ltc6810_discharge_conversion_mode_t;

typedef enum {
	LTC6810_STATE_INIT,

	// Self testing
	LTC6810_STATE_SELF_TEST_MULTIPLEXER,
	LTC6810_STATE_SELF_TEST_CELL_VOLTAGES,
	LTC6810_STATE_SELF_TEST_AUX_VOLTAGES,
	LTC6810_STATE_SELF_TEST_STATUS_VOLTAGES,
	LTC6810_STATE_SELF_TEST_OPEN_WIRE,

	// Normal operation
	LTC6810_STATE_MEASURE_CELL_VOLTAGES,
	LTC6810_STATE_MEASURE_AUX_VOLTAGES,
	LTC6810_STATE_MEASURE_REDUNDANT_CELL_VOLTAGES,
	LTC6810_STATE_MEASURE_STATUS_VOLTAGES
} ltc6810_driver_state_t;

typedef struct {
	int bus;
	int cs;
	uint8_t numICs;

	// State machine
	bool ready;
	int updateCounter;
	ltc6810_driver_state_t state;
	bool firstStep;
	util_time_t stateTime;

	// Configuration
	ltc6810_adc_mode_t adc_mode;
	ltc6810_discharge_conversion_mode_t discharge_mode;
} ltc6810_chain_t;

#pragma pack(push, 1)
typedef struct {
	bool ADCOPT : 1;
	bool DTEN : 1;
	bool REFON : 1;
	bool GPIO1 : 1;
	bool GPIO2 : 1;
	bool GPIO3 : 1;
	bool GPIO4 : 1;
	bool RSVD : 1;
	uint16_t VUV : 12;
	uint16_t VOV : 12;
	bool DCC1 : 1;
	bool DCC2 : 1;
	bool DCC3 : 1;
	bool DCC4 : 1;
	bool DCC5 : 1;
	bool DCC6 : 1;
	bool MCAL : 1;
	bool DCC0 : 1;
	bool DTMEN : 1;
	bool DIS_RED : 1;
	bool FDRF : 1;
	bool SCONV : 1;
	uint8_t DCTO : 4;
} ltc6810_config_t;

typedef struct {
	uint16_t S0V;
	uint16_t G1V;
	uint16_t G2V;
} ltc6810_aux_a_t;

typedef struct {
	uint16_t G3V;
	uint16_t G4V;
	uint16_t REF;
} ltc6810_aux_b_t;

typedef struct {
	uint16_t SC;
	uint16_t ITMP;
	uint16_t VA;
} ltc6810_status_a_t;

typedef struct {
	uint16_t VD;
	bool C1UV : 1;
	bool C1OV : 1;
	bool C2UV : 1;
	bool C2OV : 1;
	bool C3UV : 1;
	bool C3OV : 1;
	bool C4UV : 1;
	bool C4OV : 1;
	bool C5UV : 1;
	bool C5OV : 1;
	bool C6UV : 1;
	bool C6OV : 1;
	bool MUTE : 1;
	uint8_t : 3;
	uint8_t : 8;
	bool THSD : 1;
	bool MUXFAIL : 1;
	uint8_t : 2;
	uint8_t REV : 4;
} ltc6810_status_b_t;
#pragma pack(pop)

typedef struct {
	bool pec_error;
	bool self_test_cell_voltages_fail;
	bool self_test_aux_voltages_fail;
	bool self_test_status_fail;
	bool thermal_shutdown;
	bool ref_voltage_wrong;
	bool analog_power_supply_wrong;
	bool digital_power_supply_wrong;
	bool multiplexer_self_test_fail;
	bool cell_voltage_error;
	bool cell_voltage_any_invalid;
	bool cell_voltage_invalid[LTC6810_CELL_COUNT];
	bool open_wire_error;
	bool open_wire_c[LTC6810_C_PIN_COUNT];
} ltc6810_error_t;

typedef struct {
	// Registers of device
	uint64_t sid;
	ltc6810_config_t config;
	ltc6810_status_a_t status_a;
	ltc6810_status_b_t status_b;
	ltc6810_aux_a_t aux_a;
	ltc6810_aux_b_t aux_b;
	uint16_t comm[3];
	uint16_t cellVoltage_a[3];
	uint16_t cellVoltage_b[3];
	uint16_t redunantCellVoltage_a[3];
	uint16_t redunantCellVoltage_b[3];

	// Errors
	ltc6810_error_t error;
	int pec_error_counter;
} ltc6810_t;

bool ltc6810_init(ltc6810_chain_t* chain, ltc6810_t* devices);
bool ltc6810_wakeup(ltc6810_chain_t* chain);

uint8_t ltc6810_convert_adc_mode(ltc6810_adc_mode_t mode);
util_time_t ltc6810_get_adc_time(ltc6810_chain_t* chain);

bool ltc6810_has_error(const ltc6810_t device);
float ltc6810_get_die_temperature(const ltc6810_t device);
float ltc6810_get_ref_voltage(const ltc6810_t device);
float ltc6810_get_analog_supply_voltage(const ltc6810_t device);
float ltc6810_get_digital_supply_voltage(const ltc6810_t device);
float ltc6810_get_cell_voltage(const ltc6810_t device, uint8_t index);
float ltc6810_get_redundant_cell_voltage(const ltc6810_t device, uint8_t index);

bool ltc6810_enable_redundant_voltage(ltc6810_chain_t* chain, ltc6810_t* device, bool enable);
bool ltc6810_set_undervoltage(ltc6810_chain_t* chain, ltc6810_t* devices, float voltage);
bool ltc6810_set_overvoltage(ltc6810_chain_t* chain, ltc6810_t* devices, float voltage);

bool ltc6810_set_gpio(ltc6810_chain_t* chain, ltc6810_t* devices, ltc6810_t* device, uint8_t gpio, bool state);

bool ltc6810_update(ltc6810_chain_t* chain, ltc6810_t* devices);

bool ltc6810_I2C_read16(ltc6810_chain_t* chain, ltc6810_t* devices, uint8_t addr, uint8_t reg, uint16_t* data, size_t dsize);
bool ltc6810_I2C_read8(ltc6810_chain_t* chain, ltc6810_t* devices, uint8_t addr, uint8_t reg, uint8_t* data, size_t dsize);
bool ltc6810_I2C_write16(ltc6810_chain_t* chain, ltc6810_t* devices, uint8_t addr, uint8_t reg, uint16_t* data, size_t dsize);
bool ltc6810_I2C_write8(ltc6810_chain_t* chain, ltc6810_t* devices, uint8_t addr, uint8_t reg, uint8_t* data, size_t dsize);


/*
 * Dumps the chain of devices to the UART to help debugging.
 */
void ltc6810_dump(ltc6810_chain_t* chain, ltc6810_t* devices, int uart);

#ifdef __cplusplus
}
#endif

#endif
