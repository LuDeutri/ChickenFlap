#ifndef DART_DRIVER_BMP280_H_
#define DART_DRIVER_BMP280_H_


#include "../../../chickenFlap/dart/digital.h"
#include "../../../chickenFlap/dart/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
} bmp280_calibration_t;


typedef struct {
	int bus;
	int cs;

	bmp280_calibration_t calibration;
	uint8_t register_ctrl_meas;
	uint8_t register_config;

	int32_t t_fine;
} bmp280_device_t;


#define BMP280_REG_ID 0xD0
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_STATUS 0xF3
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_PRESS 0xF7
#define BMP280_REG_TEMP 0xFA
#define BMP280_REG_DIG_T1 0x88
#define BMP280_REG_DIG_T2 0x8A
#define BMP280_REG_DIG_T3 0x8C
#define BMP280_REG_DIG_P1 0x8E
#define BMP280_REG_DIG_P2 0x90
#define BMP280_REG_DIG_P3 0x92
#define BMP280_REG_DIG_P4 0x94
#define BMP280_REG_DIG_P5 0x96
#define BMP280_REG_DIG_P6 0x98
#define BMP280_REG_DIG_P7 0x9A
#define BMP280_REG_DIG_P8 0x9C
#define BMP280_REG_DIG_P9 0x9E

#define BMP280_CHIP_ID 0x58
#define BMP280_RESET_VALUE 0xB6

#define BMP280_MODE_SLEEP 0
#define BMP280_MODE_FORCED 1
#define BMP280_MODE_NORMAL 3
#define BMP280_CLEAR_MODE 3

#define BMP280_T_STANDBY_0_5ms 0
#define BMP280_T_STANDBY_62_5ms 1
#define BMP280_T_STANDBY_125ms 2
#define BMP280_T_STANDBY_250ms 3
#define BMP280_T_STANDBY_500ms 4
#define BMP280_T_STANDBY_1000ms 5
#define BMP280_T_STANDBY_2000ms 6
#define BMP280_T_STANDBY_4000ms 7
#define BMP280_CLEAR_T_STANDBY 7

#define BMP280_PRESSURE_OVERSAMPLING_SKIP 0
#define BMP280_PRESSURE_OVERSAMPLING_1 1
#define BMP280_PRESSURE_OVERSAMPLING_2 2
#define BMP280_PRESSURE_OVERSAMPLING_4 3
#define BMP280_PRESSURE_OVERSAMPLING_8 4
#define BMP280_PRESSURE_OVERSAMPLING_16 5
#define BMP280_CLEAR_PRESSURE_OVERSAMPLING 7

#define BMP280_TEMPERATURE_OVERSAMPLING_SKIP 0
#define BMP280_TEMPERATURE_OVERSAMPLING_1 1
#define BMP280_TEMPERATURE_OVERSAMPLING_2 2
#define BMP280_TEMPERATURE_OVERSAMPLING_4 3
#define BMP280_TEMPERATURE_OVERSAMPLING_8 4
#define BMP280_TEMPERATURE_OVERSAMPLING_16 5
#define BMP280_CLEAR_TEMPERATURE_OVERSAMPLING 7

#define BMP280_FILTER_OFF 0
#define BMP280_FILTER_2 1
#define BMP280_FILTER_4 2
#define BMP280_FILTER_8 3
#define BMP280_FILTER_16 4
#define BMP280_CLEAR_FILTER 7


bool bmp280_init(bmp280_device_t* device);
bool bmp280_reset(bmp280_device_t* device);
bool bmp280_get_chip_id(bmp280_device_t* device, uint8_t* id);

bool bmp280_set_mode(bmp280_device_t* device, uint8_t mode);
bool bmp280_set_t_standby(bmp280_device_t* device, uint8_t t_standby);
bool bmp280_set_pressure_oversampling(bmp280_device_t* device, uint8_t oversampling);
bool bmp280_set_temperature_oversampling(bmp280_device_t* device, uint8_t temperature);
bool bmp280_set_filter(bmp280_device_t* device, uint8_t filter);

bool bmp280_read_temperature(bmp280_device_t* device, float* temperature);
bool bmp280_read_pressure(bmp280_device_t* device, float* pressure);

#ifdef __cplusplus
}
#endif

#endif
