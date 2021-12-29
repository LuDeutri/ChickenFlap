#include "../../chickenFlap/dart/scheduler.h"

/**
 * Saves all CAN messages that are sent by the scheduler.
 */
schedulerItem_t schedulerItems[SCHEDULER_MAX_ITEM_COUNT];

/**
 * Count of the CAN messages that are saved in schedulerItems.
 * MUST be smaller than or equal to SCHEDULER_MAX_ITEM_COUNT.
 */
uint32_t schedulerItemCount;

void scheduler_init() {
    schedulerItemCount = 0;
    memset(schedulerItems, 0, sizeof(schedulerItems));
}

void scheduler_update() {
    for (uint32_t i = 0; i < schedulerItemCount; i++) {
        schedulerItem_t* item = &schedulerItems[i];
        if (hasTimerElapsed(&item->timer)) {
            // Is CAN task?
            if (item->isCANTask)
                item->sendCallback(item->message);
            else
                item->callback(item->arg);
        }
    }
}

void schedule_task(uint32_t period, uint32_t offset, TaskCallback callback, void* arg) {
    DART_NOT_NULL(callback, DART_ERROR_VALUE_IS_NULL);

    // Ensure enough space left for new item
    DART_ASSERT(schedulerItemCount < SCHEDULER_MAX_ITEM_COUNT, DART_ERROR_BUFFER_OVERFLOW);

    schedulerItem_t item;
    item.timer = createTimer(period, offset);
    item.isCANTask = false;
    item.arg = arg;
    item.callback = callback;
    memset(&item.message, 0, sizeof(item.message));
    item.sendCallback = NULL;

    // Save new item
    schedulerItems[schedulerItemCount++] = item;
}

CANMessage_t schedule_message(int can, uint16_t id, uint8_t dataLength, uint32_t period, uint32_t offset, SendCallback callback) {
    CANMessage_t dummy = {0};
    DART_NOT_NULL_RETURN(callback, DART_ERROR_VALUE_IS_NULL, dummy);

    // Ensure enough space left for new item
    DART_ASSERT_RETURN(schedulerItemCount < SCHEDULER_MAX_ITEM_COUNT, DART_ERROR_BUFFER_OVERFLOW, dummy);

    schedulerItem_t item;
    item.timer = createTimer(period, offset);
    item.isCANTask = true;
    item.arg = NULL;
    item.message = can_createTransmitMessage(can, id, dataLength);
    item.callback = NULL;
    item.sendCallback = callback;

    // Save new item
    schedulerItems[schedulerItemCount++] = item;

    // Return our created message
    return item.message;
}
