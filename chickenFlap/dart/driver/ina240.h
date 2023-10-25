#ifndef DART_DRIVER_INA240_H_
#define DART_DRIVER_INA240_H_

#include "../../../chickenFlap/dart/analog.h"

// Gain
#define INA240A1 20
#define INA240A2 50
#define INA240A3 100
#define INA240A4 200

// Dont need to set
typedef struct {
	uint16_t measuredValue; 		// measured value by the adc
	uint32_t sumMeasuredValue;		// sum of measured values for averaged one
	double sumCalculatedCurrent;	// sum of right calculated current value for averaged one
}ina240_interim_values_t;

// Dont need to set
typedef struct {
	uint8_t calibrateRound;		// runs during calibration
	uint8_t averageRound;		// runs during averaging the right current value
} ina240_counter_t;

// With x marked is need to set
typedef struct {
	uint16_t offset;					// calculated offset, set by INA240Calibration(), 0 if method is not used
	uint8_t adcChannel; 				// x // number of the adc channel to which ina240 is connected
	uint16_t adcMaxValue;				// x // max adc value ( need for current calculation )
	uint8_t gain;						// x // gain of the used ina240 chip, see defined values on the top
	float shuntResistance;				// x // Resistance of the used shunt in ohm
	bool updated;					// Can be used to send the data to master. Will be true if the current value is up to date by using the averaged current measurement.
	ina240_counter_t counter;
	ina240_interim_values_t interimValues;
} ina240_device_t;

bool ina240_init(ina240_device_t*);

// Calculate an averaged offset with 50 measured values (without considering of needed time to measure the adc value)
// Must used in the main loop
bool ina240_calibration(ina240_device_t*);

bool ina240_calculation(ina240_device_t*, float* data);
bool ina240_averaged_calculation(ina240_device_t*, float* data);

// Calculate an averaged current about 100 measures
bool ina240_read_averagedCurrent(ina240_device_t*, float* data);
bool ina240_read_Current(ina240_device_t*, float* data);

#endif
