#ifndef DART_FILESYSTEM_H_
#define DART_FILESYSTEM_H_

#include "../../chickenFlap/dart/filesystem_result.h"
#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/datetime.h"
#include "../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the DART file system wrapper. dartFilesystem_init() MUST be called before any other file system function.
 */
void dartFilesystem_init();

/**
 * Updates the DART file system wrapper. dartFilesystem_init() MUST be called before this function can be used.
 */
void dartFilesystem_update();

/**
 * Returns true, when the system recognizes a SD-card in any SD-card slot.
 */
bool dartFilesystem_is_sd_card_present();

/**
 * Converts the given file system operation return code to a user-friendly string.
 */
const char* dartFilesystem_error_str(dart_fs_result result);

/**
 * Sets the time stamp for the file. May not work on all hardware.
 */
dart_fs_result dartFilesystem_set_timestamp(char *obj, datetime_t time);

#ifdef __cplusplus
}
#endif

#endif
