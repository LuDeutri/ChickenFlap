#include "../../../../../chickenFlap/dart/hal_wrapper/driver/fatfs/fatfs_filesystem.h"

#ifdef HAL_ENABLE_FILESYSTEM_FATFS
void dartFilesystem_init() {

}

void dartFilesystem_update() {

}

bool dartFilesystem_is_sd_card_present() {
	return BSP_PlatformIsDetected() == SD_PRESENT;
}

dart_fs_result dartFilesystem_set_timestamp(char *obj, datetime_t time)
{
    FILINFO fno;
    fno.fdate = (WORD)(((time.year - 1980) * 512U) | time.month * 32U | time.day);
    fno.ftime = (WORD)(time.hours * 2048U | time.minutes * 32U | time.seconds / 2U);

    return f_utime(obj, &fno);
}

#endif
