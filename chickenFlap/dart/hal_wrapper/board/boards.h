#ifndef DART_HAL_WRAPPER_BOARD_BOARDS_H_
#define DART_HAL_WRAPPER_BOARD_BOARDS_H_

#ifdef __cplusplus
extern "C" {
#endif

void board_init();

#ifdef __cplusplus
}
#endif

// Generic
#include "../../../../chickenFlap/dart/hal_wrapper/board/atmega32m1/board_atmega32m1_generic.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/board_stm32f103_devboard.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/board_stm32f103_test_generic.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f7/board_stm32f769i_disco.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/tms320/board_tms320_launchpad.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/windows/board_vcu_windows.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/linux/board_vcu_linux.h"

// 2019
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/ny2019/board_ny_lv_pack.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/tms570/board_ams_2019_r1.h"

// xi2020
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_sensornode_r1.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_sensornode_r2.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_assi_controller_r1.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_fusebox_r1.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_lvbms_r1.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_ebs_r1.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2020/board_status_indicator.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f7/board_vcu_r1.h"

// xi2021
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2021/board_fusebox_r2.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2021/board_sensornode_r3.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/xi2021/board_dv_r1.h"
#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f7/board_ams_r1.h"

#include "../../../../chickenFlap/dart/hal_wrapper/board/stm32f1/board_chickenFlap_r1.h"

#endif
