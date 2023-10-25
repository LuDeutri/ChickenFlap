#include "../../../../chickenFlap/dart/hal_wrapper/linux/filesystem.h"

#ifdef USE_LINUX
void dartFilesystem_init() {

}

void dartFilesystem_update() {

}

bool dartFilesystem_is_sd_card_present() {
	return false;
}

dart_fs_result dartFilesystem_set_timestamp(char* obj, datetime_t time)
{
	// Do nothing
	return FR_OK;
}

dart_fs_result f_mount(FATFS* fs, const char* path, uint8_t opt) {
	DART_NOT_NULL_RETURN(path, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	return FR_OK;
}

dart_fs_result f_setlabel(const char* name) {
	DART_NOT_NULL_RETURN(name, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	return FR_OK;
}

dart_fs_result f_open(FIL* fs, const char* path, uint8_t mode) {
	DART_NOT_NULL_RETURN(fs, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	DART_NOT_NULL_RETURN(path, DART_ERROR_INVALID_VALUE, FR_INT_ERR);

	if (mode == FA_READ)
		fs = fopen(path, "rb");
	else if (mode == FA_WRITE)
        fs = fopen(path, "wb");
	else if (mode == FA_OPEN_APPEND)
        fs = fopen(path, "ab");
	else
        fs = fopen(path, "wb+");

	return fs != NULL ? FR_OK : FR_DISK_ERR;
}

dart_fs_result f_sync(FIL* fs) {
	DART_NOT_NULL_RETURN(fs, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	fflush(fs);
	return FR_OK;
}

dart_fs_result f_close(FIL* fs) {
	DART_NOT_NULL_RETURN(fs, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	fclose(fs);
	return FR_OK;
}

dart_fs_result f_mkdir(const char* path) {
	DART_NOT_NULL_RETURN(path, DART_ERROR_INVALID_VALUE, FR_INT_ERR);

    int res = mkdir(path, 0700);
	if (res == 0 || res == EEXIST)
		return FR_OK;
	return FR_DISK_ERR;
}

dart_fs_result f_stat(const char* path, void* finfo) {
	DART_NOT_NULL_RETURN(path, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	if (access(path, F_OK) != 0)
		return FR_NO_FILE;
	return FR_OK;
}

dart_fs_result f_read(FIL* fp, void* buff, uint32_t btr, uint32_t* br) {
	DART_NOT_NULL_RETURN(fp, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	DART_NOT_NULL_RETURN(buff, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	DART_NOT_NULL_RETURN(br, DART_ERROR_INVALID_VALUE, FR_INT_ERR);

	*br = sizeof(uint8_t) * fread((uint8_t*)buff, sizeof(uint8_t), btr, fp);
	return FR_OK;
}

dart_fs_result f_write(FIL* fp, const void* buff, uint32_t btw, uint32_t* bw) {
	DART_NOT_NULL_RETURN(fp, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	DART_NOT_NULL_RETURN(buff, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	DART_NOT_NULL_RETURN(bw, DART_ERROR_INVALID_VALUE, FR_INT_ERR);

	*bw = sizeof(uint8_t) * fwrite((uint8_t*)buff, sizeof(uint8_t), btw, fp);
	return FR_OK;
}

dart_fs_result f_mkfs(const char* path, const void* opt, uint8_t e, void* work, uint32_t len) {
	DART_NOT_NULL_RETURN(path, DART_ERROR_INVALID_VALUE, FR_INT_ERR);
	DART_NOT_NULL_RETURN(work, DART_ERROR_INVALID_VALUE, FR_INT_ERR);

	// Do nothing
	return FR_OK;
}

dart_fs_result f_expand(FIL* fp, uint64_t szf, uint8_t opt) {
	DART_NOT_NULL_RETURN(fp, DART_ERROR_INVALID_VALUE, FR_INT_ERR);

	// Do nothing
	return FR_OK;
}
#endif
