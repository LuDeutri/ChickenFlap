#ifndef DART_HAL_WRAPPER_DRIVER_MCP2515_CAN_H_
#define DART_HAL_WRAPPER_DRIVER_MCP2515_CAN_H_

#include "../../../../../chickenFlap/dart/hal_wrapper/board/boards.h"

#ifdef HAL_ENABLE_CAN_MCP2515

#include "../../../../../chickenFlap/dart/can.h"
#include "../../../../../chickenFlap/dart/spi.h"

#include "../../../../../chickenFlap/dart/hal_wrapper/driver/mcp2515/mcp2515.h"

bool hal_can_mcp2515_send(MCP2515* mcp, CANBufferItem_t* item);
bool hal_can_mcp2515_receive(int can, MCP2515* mcp, CANBufferItem_t* item);

#ifdef HAL_ENABLE_FREERTOS
//#define HAL_ENABLE_MCP2515_THREAD

#ifdef HAL_ENABLE_MCP2515_THREAD
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "cmsis_os.h"

void vTaskCAN(void* pvParameters);
#endif
#endif
#endif

#endif
