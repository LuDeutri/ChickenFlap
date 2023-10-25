#include "../../../chickenFlap/dart/driver/ina240.h"

bool ina240_init(ina240_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	device->offset = 0;
	device->updated = false;
	device->interimValues.sumCalculatedCurrent = 0;
	device->interimValues.measuredValue = 0;
	device->interimValues.sumMeasuredValue = 0;

	device->counter.averageRound = 0;
	device->counter.calibrateRound = 0;

	device->interimValues.measuredValue = getADC(device->adcChannel);
	return true;
}


bool ina240_calibration(ina240_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	if (device->counter.calibrateRound > 101) { // Already calibrated
		return true;
	}

	device->interimValues.measuredValue = getADC(device->adcChannel);

	if (device->interimValues.measuredValue == 0) { // Wait one more round until the value is measured
	}
	else {
		device->counter.calibrateRound++;
		device->interimValues.sumMeasuredValue += device->interimValues.measuredValue;
	}
	if (device->counter.calibrateRound > 100) {
		device->offset = device->interimValues.sumMeasuredValue / device->counter.calibrateRound;
		return true;
	}
	return false;
}

bool ina240_averaged_calculation(ina240_device_t* device, float* data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	double currentADCVoltage = ((device->interimValues.measuredValue - device->offset) / (double)device->adcMaxValue) * 3.3;
	double calculatedCurrent = (currentADCVoltage / (device->gain * device->shuntResistance));

	// Averange the calculated Current
	device->interimValues.sumCalculatedCurrent += calculatedCurrent;
	device->counter.averageRound++;
	if (device->counter.averageRound > 100) {
		*data = ((float)(device->interimValues.sumCalculatedCurrent / device->counter.averageRound)) * (-1);
		device->updated = true;

		// Restart counter, interim value
		device->counter.averageRound = 0;
		device->interimValues.sumCalculatedCurrent = 0;
		return true;
	}
	return false;
}

bool ina240_calculation(ina240_device_t* device, float* data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	double currentADCVoltage = ((device->interimValues.measuredValue - device->offset) / (double)device->adcMaxValue) * 3.3;
	double calculatedCurrent = currentADCVoltage / (device->gain * device->shuntResistance);
	*data = ((float)(calculatedCurrent))*(-1);

	return true;
}

bool ina240_read_averagedCurrent(ina240_device_t* device, float* data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	device->updated = false;

	if(device->offset == 0)  // still not calibrated
		return false;

	device->interimValues.measuredValue = getADC(device->adcChannel);

	if(!ina240_averaged_calculation(device, data))
		return false;

	return true;
}

bool ina240_read_Current(ina240_device_t* device, float* data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	device->interimValues.measuredValue = getADC(device->adcChannel);

	if(!ina240_calculation(device, data))
			return false;

	return true;
}
