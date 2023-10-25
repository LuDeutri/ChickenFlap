#ifndef DART_HAL_WRAPPER_WINDOWS_FILESYSTEM_H_
#define DART_HAL_WRAPPER_WINDOWS_FILESYSTEM_H_

#include "../../../../chickenFlap/dart/filesystem.h"

#ifdef USE_WINDOWS
#include <fileapi.h>
#include <direct.h>
#include "../../../../chickenFlap/dart/filesystem_result.h"

typedef struct {
	int dummy;
} windows_filesystem_t;

#define FIL FILE*
#define FATFS windows_filesystem_t

#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30

#define FM_ANY NULL

#ifdef __cplusplus
extern "C" {
#endif

	dart_fs_result f_mount(FATFS* fs, const char* path, uint8_t opt);
	dart_fs_result f_setlabel(const char* name);
	dart_fs_result f_open(FIL* fs, const char* path, uint8_t mode);
	dart_fs_result f_sync(FIL* fs);
	dart_fs_result f_close(FIL* fs);
	dart_fs_result f_mkdir(const char* path);
	dart_fs_result f_stat(const char* path, void* finfo);

	dart_fs_result f_read(FIL* fp, void* buff, uint32_t btr, uint32_t* br);
	dart_fs_result f_write(FIL* fp, const void* buff, uint32_t btw, uint32_t* bw);

	dart_fs_result f_mkfs(const char* path, const void* opt, uint8_t e, void* work, uint32_t len);
	dart_fs_result f_expand(FIL* fp, uint64_t szf, uint8_t opt);
#ifdef __cplusplus
}
#endif

#endif
#endif
