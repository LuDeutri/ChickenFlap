#ifndef DART_HAL_WRAPPER_DRIVER_FATFS_FATFS_FILESYSTEM_H_
#define DART_HAL_WRAPPER_DRIVER_FATFS_FATFS_FILESYSTEM_H_

#include "../../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef HAL_ENABLE_FILESYSTEM_FATFS
// Include first FATFS
#include <fatfs.h>

// then include our filesystem header
#include "../../../../../chickenFlap/dart/filesystem.h"
#endif

#endif
