#include "../../../chickenFlap/dart/util/calibration.h"

calibration_t calibration_init(int32_t min, int32_t max) {
	calibration_t c;
	c.min = min;
	c.max = max;
	c.fmin = 0.0f;
	c.fmax = 1.0f;
	calibration_reset(&c);
	return c;
}

calibration_t calibration_finit(float min, float max) {
	calibration_t c;
	c.min = 0;
	c.max = 1;
	c.fmin = min;
	c.fmax = max;
	calibration_reset(&c);
	return c;
}

void calibration_reset(calibration_t* calib) {
	DART_NOT_NULL(calib, DART_ERROR_UTIL);
	calib->reset = true;
	calib->old_min = calib->min;
	calib->old_max = calib->max;
	calib->old_fmin = calib->fmin;
	calib->old_fmax = calib->fmax;
}

void calibration_get(calibration_t* calib, int32_t* min, int32_t* max) {
	DART_NOT_NULL(calib, DART_ERROR_UTIL);
	DART_NOT_NULL(min, DART_ERROR_UTIL);
	DART_NOT_NULL(max, DART_ERROR_UTIL);

	// Check if the calibration has been updated since last calibration
	if (abs(calib->min - calib->max) < 10) {
		*min = calib->old_min;
		*max = calib->old_max;
	}
	else {
		*min = calib->min;
		*max = calib->max;
	}
}

void calibration_fget(calibration_t* calib, float* min, float* max) {
	DART_NOT_NULL(calib, DART_ERROR_UTIL);
	DART_NOT_NULL(min, DART_ERROR_UTIL);
	DART_NOT_NULL(max, DART_ERROR_UTIL);

	// Check if the calibration has been updated since last calibration
	if (fabs(calib->fmin - calib->fmax) < 0.01) {
		*min = calib->old_fmin;
		*max = calib->old_fmax;
	}
	else {
		*min = calib->fmin;
		*max = calib->fmax;
	}
}

void calibration_update(calibration_t* calib, int32_t value) {
	DART_NOT_NULL(calib, DART_ERROR_UTIL);
	// Check if this is the first sample
	if (calib->reset) {
		calib->reset = false;
		calib->min = value;
		calib->max = value;
	}
	else {
		calib->min = min(calib->min, value);
		calib->max = max(calib->max, value);
	}
}

void calibration_fupdate(calibration_t* calib, float value) {
	DART_NOT_NULL(calib, DART_ERROR_UTIL);
	// Check if this is the first sample
	if (calib->reset) {
		calib->reset = false;
		calib->fmin = value;
		calib->fmax = value;
	}
	else {
		calib->fmin = min(calib->fmin, value);
		calib->fmax = max(calib->fmax, value);
	}
}

float calibration_map(calibration_t* calib, int32_t value) {
	DART_NOT_NULL_RETURN(calib, DART_ERROR_UTIL, 0.0f);

	int32_t min, max;
	calibration_get(calib, &min, &max);

	if (value <= min)
		return 0.0f;
	else if (value >= max)
		return 1.0f;
	return ((float)(value - min)) / ((float)(max - min));
}

float calibration_fmap(calibration_t* calib, float value) {
	DART_NOT_NULL_RETURN(calib, DART_ERROR_UTIL, 0.0f);

	float min, max;
	calibration_fget(calib, &min, &max);

	if (value <= min)
		return 0.0f;
	else if (value >= max)
		return 1.0f;
	return (value - min) / (max - min);
}
