#include "../../../../chickenFlap/dart/hal_wrapper/tms570/can.h"

#if defined(USE_TMS570) && defined(HAL_ENABLE_CAN)

#ifndef CAN_MAP
#error "CAN_MAP is not defined"
#endif


canBASE_t* canHandles[CAN_MAX_CHANNEL_COUNT] = CAN_MAP;

/**
 * HAL configuration value indicating the message is received.
 */
#define CAN_RX 0x00000000U
/**
 * HAL configuration value indicating the message is transmitted.
 */
#define CAN_TX 0x20000000U

/**
 * HAL configuration value indicating that no interrupt is used.
 */
#define CAN_INTERRUPT_DISABLE 0x00000000U
/**
 * HAL configuration value indicating that only the receive interrupt is used.
 */
#define CAN_INTERRUPT_RX 0x00000400U
/**
 * HAL configuration value indicating that only the transmit interrupt is used.
 */
#define CAN_INTERRUPT_TX 0x00000800U

#define CAN_MASK_ALL 0x00000000U
#define CAN_MASK_NORMAL 0x000007FFU

/**
 * Counter used for populating the messageBox field of the CAN messages. Incremented for each new message.
 */
uint8_t can_messageBoxCounter = canMESSAGE_BOX2; // canMESSAGE_BOX1 is reserved for RX

void hal_can_initMessageHAL(CANMessage_t message, uint32_t canDir, uint32_t irq, uint32_t mask) {
    DART_ASSERT(mask <= CAN_MAX_ID, DART_ERROR_INVALID_VALUE);

    // Construct values that are set for the registers
    canBASE_t* canReg = canHandles[message.can];
    DART_NOT_NULL(canReg, DART_ERROR_VALUE_IS_NULL);

    uint32_t msk = (uint32_t)(0xC0000000U | ((mask & CAN_MAX_ID) << 18));
    uint32_t arb = (uint32_t)(0x80000000U | canDir | ((message.id & CAN_MAX_ID) << 18));
    uint32_t ctl = (uint32_t)(0x00001080U | irq | message.dataLength);

    // Set state for initialize
    canReg->CTL = (uint32_t)(0x00000200U | (0x00000005U << 10) | 0x00020043U);

    // Use IF2 if messageBox number is even, otherwise use IF1 for canMessageBox initialization
    if (message.mailbox % 2 == 0) {
        // IF2 is used
        // Wait till HAL is ready
        while ((canReg->IF2STAT & 0x80U) == 0x80U);

        // Set HAL registers
        canReg->IF2MSK = msk;
        canReg->IF2ARB = arb;
        canReg->IF2MCTL = ctl;
        canReg->IF2CMD = 0xF8U;
        canReg->IF2NO = message.mailbox;
    }
    else {
        // IF1 is used
        // Wait till HAL is ready
        while ((canReg->IF1STAT & 0x80U) == 0x80U);

        // Set HAL registers
        canReg->IF1MSK = msk;
        canReg->IF1ARB = arb;
        canReg->IF1MCTL = ctl;
        canReg->IF1CMD = 0xF8U;
        canReg->IF1NO = message.mailbox;
    }
    // Done with initialize
    canREG1->CTL &= ~(uint32_t)(0x00000041U);
}

void hal_can_init() {
    // Initialize HAL
    canInit();

    // Create dummy message for an interrupt (canMessageNotification) that is called when any CAN message is received
    for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++) {
        if (canHandles[i] == NULL)
            continue;

        CANMessage_t dummyRX;
        dummyRX.can = i;
        dummyRX.mailbox = canMESSAGE_BOX1;
        dummyRX.id = 0b01010101;
        dummyRX.dataLength = CAN_MAX_DATA_SIZE;
        hal_can_initMessageHAL(dummyRX, CAN_RX, CAN_INTERRUPT_RX, CAN_MASK_ALL);
    }

    // Enable error notification
    for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++)
        if (canHandles[i] != NULL)
            canEnableErrorNotification(canHandles[i]);
}

void hal_can_update() {
    //Do nothing
}

void hal_can_yield() {
    // Do nothing
}

void hal_can_set_baudrate(const int can, const CAN_Baudrate_t baudrate) {
    DART_NOT_USED(can);
    DART_NOT_USED(baudrate);
    // Do nothing
}

void hal_can_createTransmitMessage(CANMessage_t* message) {
    message->mailbox = can_messageBoxCounter++;
    hal_can_initMessageHAL(*message, CAN_TX, CAN_INTERRUPT_DISABLE, CAN_MASK_NORMAL);
}

void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer) {
    hal_can_update();

    // Transmit 10 messages at the same time
    for (int i = 0; i < 10; i++) {
        if (rb_isempty(ringBuffer))
            break;

        // Get index from ring buffer (peek does not remove the index from the ring buffer)
        int index = rb_peek(ringBuffer);

        // Write buffered CAN message to the free mailbox on the hardware
        CANBufferItem_t* item = &buffer[index];

        // Wait till old message has been sent by HAL
        canBASE_t* handle = canHandles[item->can];
        DART_NOT_NULL(handle, DART_ERROR_VALUE_IS_NULL);

        // Wait till can send the message again
        if (canIsTxMessagePending(handle, item->mailbox))
            break;

        // Remove the index from ring buffer
        DART_NOT_USED(rb_get(ringBuffer));
        canTransmit(handle, item->mailbox, item->data);
    }
}

void hal_can_fillReceiveBuffer() {
    // Do nothing
}

bool can_has_error(int can) {
    DART_NOT_USED(can);
    return false;
}

uint32_t can_get_error_code(int can) {
    DART_NOT_USED(can);
    return 0;
}

void can_reset_error(int can) {
    DART_NOT_USED(can);
    // Do nothing
}

/**
 * Function that is called when a CAN message is received. This function is called by the HAL.
 * @param node The CAN register the message was received.
 * @param messageBox MessageBox the message was received.
 */
void canMessageNotification(canBASE_t* node, uint32_t messageBox) {
    if (node == NULL)
        return;

    // Only process notifications called by our dummy can message (see hal_can_init).
    if (messageBox == canMESSAGE_BOX1) {
        for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++) {
            if (canHandles[i] != node)
                continue;

            util_timer_t timeout = createTimerNoOffset(CAN_TIMEOUT);
            while (!canIsRxMessageArrived(node, messageBox))
                if (hasTimerElapsed(&timeout))
                    DART_ERROR(DART_ERROR_TIMEOUT);

            CANBufferItem_t item;
            item.can = i;
            item.id = ((canGetID(node, messageBox)) >> 18) & CAN_MAX_ID;
            item.dlc = CAN_MAX_DATA_SIZE;

            // Get data
            canGetData(node, messageBox, item.data);

            _dartCAN_addReceiveItem(item);
        }
    }
}
#endif
