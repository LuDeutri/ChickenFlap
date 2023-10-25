#ifndef SRC_UTIL_CALIBRATION_H_
#define SRC_UTIL_CALIBRATION_H_

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/util/error.h"
#include "../../../chickenFlap/dart/util/math.h"

typedef struct {
	// Old values saved that were set before the reset
	int32_t old_min;
	int32_t old_max;
	float old_fmin;
	float old_fmax;

	// Current updated calibration values
	int32_t min;
	int32_t max;
	float fmin;
	float fmax;

	// If the calibration was reset and no new data was written to it
	bool reset;
} calibration_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the calibration with default values.
 */
calibration_t calibration_init(int32_t min, int32_t max);

/**
 * Initializes the calibration with default float values.
 */
calibration_t calibration_finit(float min, float max);

/**
 * Gets the min and max values saved in the calibration.
 */
void calibration_get(calibration_t* calib, int32_t* min, int32_t* max);

/**
 * Gets the min and max float values saved in the calibration.
 */
void calibration_fget(calibration_t* calib, float* min, float* max);

/**
 * Resets the calibration to default values.
 */
void calibration_reset(calibration_t* calib);

/**
 * Updates the calibration with new given value.
 */
void calibration_update(calibration_t* calib, int32_t value);

/**
 * Updates the calibration with the new given float value.
 */
void calibration_fupdate(calibration_t* calib, float value);

/**
 * Maps the value to the [0, 1] interval based on the given calibration.
 */
float calibration_map(calibration_t* calib, int32_t value);

/**
 * Maps the value to the [0, 1] interval based on the given calibration.
 */
float calibration_fmap(calibration_t* calib, float value);

#ifdef __cplusplus
}
#endif

#endif
