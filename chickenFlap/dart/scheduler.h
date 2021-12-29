#ifndef DART_SCHEDULER_H_
#define DART_SCHEDULER_H_

/**
 * Implements a scheduler that can schedule the calling of functions at different rates.
 */
#include "../../chickenFlap/dart/can.h"
#include "../../chickenFlap/dart/util/error.h"
#include "../../chickenFlap/dart/util/timer.h"

/**
 * Callback that is called when a task should get updated.
 */
typedef void (*TaskCallback)(void* arg);

/**
 * Callback that is called when a CAN message should get sent.
 */
typedef void (*SendCallback)(CANMessage_t message);

/**
 * Internal data structure to keep track of all scheduled messages.
 */
typedef struct {
    util_timer_t timer;

    // Normal task
    void* arg;
    TaskCallback callback;

    // CAN task
    bool isCANTask;
    CANMessage_t message;
    SendCallback sendCallback;
} schedulerItem_t;

/**
 * Maximum amount of tasks that can be scheduled by the scheduler.
 * Schedule more messages than this value results in an error.
 */
#define SCHEDULER_MAX_ITEM_COUNT 16

/**
 * Initializes the scheduler.
 */
void scheduler_init();

/**
 * Updates the scheduler.
 * Runs all pending tasks that should get updated.
 */
void scheduler_update();

/**
 * Schedules a task that should be updated periodically. Creates the timer with the specified period and offset. When the timer elapses
 * the TaskCallback callback is called.
 * Only SCHEDULER_MAX_ITEM_COUNT tasks can be scheduled at all times. Trying to schedule more tasks results in an error.
 */
void schedule_task(uint32_t period, uint32_t offset, TaskCallback callback, void* arg);

/**
 * Schedules a CAN message that should be sent periodically.
 * Creates the CAN message with the specified id and dataLength. Creates the timer with the specified period and offset. When the timer elapses
 * the SendCallback callback is called.
 * Only SCHEDULER_MAX_ITEM_COUNT tasks can be scheduled at all times. Trying to schedule more messages results in an error.
 * Returns the newly create CAN message.
 */
CANMessage_t schedule_message(int can, uint16_t id, uint8_t dataLength, uint32_t period, uint32_t offset, SendCallback callback);

#endif
