#ifndef DART_HAL_WRAPPER_DUMMY_FILESYSTEM_H_
#define DART_HAL_WRAPPER_DUMMY_FILESYSTEM_H_

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef HAL_DUMMY_FILESYSTEM

#include "../../../../chickenFlap/dart/filesystem.h"
#include "../../../../chickenFlap/dart/filesystem_result.h"

#ifdef _FATFS
#error "Can not use dummy filesystem when FATFS is enabled"
#endif

typedef struct {

} FATFS;


typedef struct {

} FIL;

#define FA_WRITE 0
#define FA_OPEN_APPEND 0
#define FM_ANY 0


#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int	UINT;

dart_fs_result f_open (
  FIL* fp,           /* [OUT] Pointer to the file object structure */
  const char* path, /* [IN] File name */
  uint8_t mode          /* [IN] Mode flags */
);

dart_fs_result f_close (
  FIL* fp     /* [IN] Pointer to the file object */
);

dart_fs_result f_read (
  FIL* fp,     /* [IN] File object */
  void* buff,  /* [OUT] Buffer to store read data */
  uint32_t btr,    /* [IN] Number of bytes to read */
  uint32_t* br     /* [OUT] Number of bytes read */
);

dart_fs_result f_write (
  FIL* fp,          /* [IN] Pointer to the file object structure */
  const void* buff, /* [IN] Pointer to the data to be written */
  uint32_t btw,         /* [IN] Number of bytes to write */
  uint32_t* bw          /* [OUT] Pointer to the variable to return number of bytes written */
);

dart_fs_result f_sync (
  FIL* fp     /* [IN] File object */
);

dart_fs_result f_mkdir (
  const char* path /* [IN] Directory name */
);

dart_fs_result f_mount (
  FATFS*       fs,    /* [IN] Filesystem object */
  const char* path,  /* [IN] Logical drive number */
  uint8_t         opt    /* [IN] Initialization option */
);

dart_fs_result f_mkfs (
  const char*  path,  /* [IN] Logical drive number */
  const void* opt,/* [IN] Format options */
  int arg,
  void*  work,         /* [-]  Working buffer */
  uint32_t  len            /* [IN] Size of working buffer */
);

dart_fs_result f_setlabel (
  const char* label  /* [IN] Volume label to be set */
);

dart_fs_result f_stat (
  const char* path,  /* [IN] Object name */
  void* fno        /* [OUT] FILINFO structure */
);

dart_fs_result f_expand (FIL* fp, uint64_t szf, uint8_t opt);

#ifdef __cplusplus
}
#endif

#endif

#endif
