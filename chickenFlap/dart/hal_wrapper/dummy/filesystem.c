#include "../../../../chickenFlap/dart/hal_wrapper/dummy/filesystem.h"

#ifdef HAL_DUMMY_FILESYSTEM
void dartFilesystem_init() {

}

void dartFilesystem_update() {

}

bool dartFilesystem_is_sd_card_present() {
	return false;
}

dart_fs_result dartFilesystem_set_timestamp(char* obj, datetime_t time)
{
	return FR_INT_ERR;
}

dart_fs_result f_open (
  FIL* fp,           /* [OUT] Pointer to the file object structure */
  const char* path, /* [IN] File name */
  uint8_t mode          /* [IN] Mode flags */
) {
	return FR_INT_ERR;
}

dart_fs_result f_close (
  FIL* fp     /* [IN] Pointer to the file object */
) {
	return FR_INT_ERR;
}

dart_fs_result f_read (
  FIL* fp,     /* [IN] File object */
  void* buff,  /* [OUT] Buffer to store read data */
  uint32_t btr,    /* [IN] Number of bytes to read */
  uint32_t* br     /* [OUT] Number of bytes read */
) {
	return FR_INT_ERR;
}

dart_fs_result f_write (
  FIL* fp,          /* [IN] Pointer to the file object structure */
  const void* buff, /* [IN] Pointer to the data to be written */
  UINT btw,         /* [IN] Number of bytes to write */
  UINT* bw          /* [OUT] Pointer to the variable to return number of bytes written */
) {
	return FR_INT_ERR;
}

dart_fs_result f_sync (
  FIL* fp     /* [IN] File object */
) {
	return FR_INT_ERR;
}

dart_fs_result f_mkdir (
  const char* path /* [IN] Directory name */
) {
	return FR_INT_ERR;
}

dart_fs_result f_mount (
  FATFS*       fs,    /* [IN] Filesystem object */
  const char* path,  /* [IN] Logical drive number */
  uint8_t         opt    /* [IN] Initialization option */
) {
	return FR_INT_ERR;
}

dart_fs_result f_mkfs (
  const char*  path,  /* [IN] Logical drive number */
  const void* opt,/* [IN] Format options */
  int arg,
  void*  work,         /* [-]  Working buffer */
  UINT  len            /* [IN] Size of working buffer */
) {
	return FR_INT_ERR;
}

dart_fs_result f_setlabel (
  const char* label  /* [IN] Volume label to be set */
) {
	return FR_INT_ERR;
}

dart_fs_result f_stat (
  const char* path,  /* [IN] Object name */
  void* fno        /* [OUT] FILINFO structure */
) {
	return FR_INT_ERR;
}

dart_fs_result f_mkfs(const char* path, const void* opt, uint8_t e, void* work, uint32_t len) {
	return FR_INT_ERR;
}

dart_fs_result f_expand(FIL* fp, uint64_t szf, uint8_t opt) {
	return FR_INT_ERR;
}
#endif
