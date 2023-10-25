#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_i2c.h"

bool ltc68xx_i2c_check(ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data, size_t dataSize) {
    // Check chain
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(chain->numICs > 0, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(chain->devices, DART_ERROR_INVALID_VALUE, false);

    // Check data
    DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(dataSize >= chain->numICs * sizeof(ltc68xx_i2c_data_t), DART_ERROR_INVALID_VALUE, false);

    for (int i = 0; i < chain->numICs; i++) {
        // Check items of data
        ltc68xx_i2c_data_t* item = &data[i];
        DART_NOT_NULL_RETURN(item, DART_ERROR_INVALID_VALUE, false);
        if (!item->nop) {
            DART_ASSERT_RETURN(item->addr <= 127, DART_ERROR_INVALID_VALUE, false);
            DART_NOT_NULL_RETURN(item->data, DART_ERROR_INVALID_VALUE, false);
            DART_ASSERT_RETURN(item->dataSize > 0, DART_ERROR_INVALID_VALUE, false);
        }
    }
    return true;
}

bool ltc68xx_i2c_begin_read_register8(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(chain->numICs < 24, DART_ERROR_INVALID_VALUE, false); // tempData has only 24 entries

    // Write the register
    ltc68xx_i2c_data_t tempData[24];
    for (int i = 0; i < chain->numICs; i++) {
        ltc68xx_i2c_data_t* item = &tempData[i];
        item->nop = false;
        item->addr = addr;
        item->dataSize = sizeof(uint8_t);
        item->data = &reg;
    }

    if (!ltc68xx_wakeup(chain))
        return false;
    return ltc68xx_i2c_write(chain, tempData, sizeof(tempData));
}

bool _ltc68xx_i2c_read_register(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, void* data, size_t elementSize, size_t dataSize, bool* succesful, size_t successfulSize) {
    DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(dataSize >= chain->numICs * elementSize, DART_ERROR_INVALID_VALUE, false);
    DART_NOT_NULL_RETURN(succesful, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(successfulSize >= chain->numICs, DART_ERROR_INVALID_VALUE, false);

    memset(data, 0, dataSize);

    if (!ltc68xx_i2c_begin_read_register8(chain, addr, reg))
        return false;

    // Then read the data
    ltc68xx_i2c_data_t tempData[24];
    for (int i = 0; i < chain->numICs; i++) {
       ltc68xx_i2c_data_t* item = &tempData[i];
       item->nop = false;
       item->addr = addr;
       item->dataSize = elementSize;
       item->data = (((uint8_t*)data) + elementSize * i);
    }

    if (!ltc68xx_i2c_read(chain, tempData, sizeof(tempData)))
       return false;

    // Copy successful flag
    for (int i = 0; i < chain->numICs; i++)
        succesful[i] = tempData[i].successful;
    return true;
}

bool ltc68xx_i2c_read_register8(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint8_t* data, size_t dataSize, bool* successful, size_t successfulSize) {
    return _ltc68xx_i2c_read_register(chain, addr, reg, data, sizeof(uint8_t), dataSize, successful, successfulSize);
}

bool ltc68xx_i2c_read_register16(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint16_t* data, size_t dataSize, bool* successful, size_t successfulSize) {
    return _ltc68xx_i2c_read_register(chain, addr, reg, data, sizeof(uint16_t), dataSize, successful, successfulSize);
}

size_t ltc68xx_i2c_prepare_data(ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data) {
    // Set successful flag to true
    for (int i = 0; i < chain->numICs; i++)
        (&data[i])->successful = true;

    // Find maximum number of bytes to read
    size_t maxDataSize = 0;
    for (int i = 0; i < chain->numICs; i++)
        maxDataSize = max(maxDataSize, data[i].dataSize);
    return maxDataSize;
}

int ltc68xx_i2c_pad_comm(int commIndex, ltc68xx_chain_t* chain) {
    // Pad until we have again 3 commands in the comm buffer
    while (commIndex <= LTC68XX_COMM_CMD_COUNT - 1) {
        for (int j = 0; j < chain->numICs; j++) {
            ltc68xx_t* device = &chain->devices[j];
            device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_NO_TRANSMIT | LTC68XX_FCOMM_I2C_W_MASTER_NACK;
            device->commNeedsAck[commIndex] = false;
        }
        commIndex++;
    }
    return commIndex;
}

bool ltc68xx_perform_i2c_comm(ltc68xx_chain_t* chain) {
    // Clear the chain by executing dummy command
    if (!ltc68xx_wakeup(chain))
        return false;
    // Push data to the LTC chips
    if (!ltc68xx_write_comm(chain))
        return false;
    // Execute COMM commands (we need to hold the SPI bus for 9 bytes, to actually execute the I2C command)
    if (!ltc68xx_cmd_hold_clockcycles(chain, LTC68XX_CMD_STCOMM, 9))
        return false;
    // Clear the chain by executing dummy command
    if (!ltc68xx_wakeup(chain))
        return false;
    // Read result
    return ltc68xx_read_comm(chain);
}

void ltc68xx_i2c_check_and_copy_receive(size_t dataBufferIndex, ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data, bool copyData) {
    // Read response
    for (int k = 0; k < LTC68XX_COMM_CMD_COUNT; k++) {
       for (int n = 0; n < chain->numICs; n++) {
           ltc68xx_t* device = &chain->devices[n];
           ltc68xx_i2c_data_t* item = &data[n];

           // Check if the master actually transmitted data the slave can acknowledge
           uint16_t commNeedsAck = device->commNeedsAck[k];
           uint16_t commRx = device->commRx[k];

           bool slaveAck = (commRx & LTC68XX_FCOMM_MASK) == LTC68XX_FCOMM_I2C_R_SLAVE_ACK || (commRx & LTC68XX_FCOMM_MASK) == LTC68XX_FCOMM_I2C_R_MASTER_ACK;
           if (commNeedsAck && !slaveAck)
               item->successful = false;

           // Copy data
           if (copyData) {
        	   size_t dataIndex = ((dataBufferIndex - LTC68XX_COMM_CMD_COUNT) + 1) + k;
               if (dataIndex < item->dataSize)
                   item->data[dataIndex] = (commRx >> 4) & 0xFF;
           }
       }
    }
}

bool ltc68xx_i2c_read(ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data, size_t dataSize) {
    if (!ltc68xx_i2c_check(chain, data, dataSize))
        return false;

    int maxDataSize = ltc68xx_i2c_prepare_data(chain, data);

    // Generate I2C sequence
    int commIndex = 0;

    // We iterate over -1 to size
    // i == -1 is the start bit + the address
    for (int i = -1; i < maxDataSize; i++) {
        for (int j = 0; j < chain->numICs; j++) {
           ltc68xx_t* device = &chain->devices[j];
           ltc68xx_i2c_data_t* item = &data[j];

           if (i == -1) { // start + address
               uint8_t addr = ((item->addr << 1) | 1); // I2C address byte (read = bit set at end)
               device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_START | LTC68XX_FCOMM_I2C_W_MASTER_NACK | (addr << 4);
               device->commNeedsAck[commIndex] = true;
           }
           else if (i < (int)item->dataSize - 1) { // data byte
               device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_BLANK | LTC68XX_FCOMM_I2C_W_MASTER_ACK | 0xFF << 4;
               device->commNeedsAck[commIndex] = false;
           }
           else if (i == (int)item->dataSize - 1) { // data byte + stop
               device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_BLANK | LTC68XX_FCOMM_I2C_W_MASTER_NACK_STOP | 0xFF << 4;
               device->commNeedsAck[commIndex] = false;
           }
           else { // transmit nothing
               device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_NO_TRANSMIT | LTC68XX_FCOMM_I2C_W_MASTER_NACK;
               device->commNeedsAck[commIndex] = false;
           }
        }
        commIndex++;

        // Pad communication registers by transmitting nothing (at the end)
        if (i == maxDataSize - 1)
            commIndex = ltc68xx_i2c_pad_comm(commIndex, chain);

        // Every 3 commands we need to write, start the communication and read the result
        if (commIndex >= LTC68XX_COMM_CMD_COUNT) {
           ltc68xx_perform_i2c_comm(chain);
           ltc68xx_i2c_check_and_copy_receive(i, chain, data, true);

           // Reset index
           commIndex = 0;
       }
   }
   return true;
}

bool ltc68xx_i2c_write_register(ltc68xx_chain_t* chain, uint8_t addr, uint8_t* dataToWrite, size_t dataToWriteSize, bool* successful, size_t successfulSize) {
   DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, false);
   DART_ASSERT_RETURN(chain->numICs < 24, DART_ERROR_INVALID_VALUE, false); // tempData has only 24 entries
   DART_NOT_NULL_RETURN(successful, DART_ERROR_INVALID_VALUE, false);
   DART_ASSERT_RETURN(successfulSize >= chain->numICs, DART_ERROR_INVALID_VALUE, false);

   // Build temp data
   ltc68xx_i2c_data_t tempData[24];
   for (int i = 0; i < chain->numICs; i++) {
       ltc68xx_i2c_data_t* item = &tempData[i];
       item->nop = false;
       item->addr = addr;
       item->dataSize = dataToWriteSize;
       item->data = dataToWrite;
   }

   if (!ltc68xx_i2c_write(chain, tempData, sizeof(tempData)))
       return false;

   // Copy successful flag
   for (int i = 0; i < chain->numICs; i++)
       successful[i] = tempData[i].successful;
   return true;
}

bool ltc68xx_i2c_write_register_no_data(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, bool* successful, size_t successfulSize) {
    uint8_t dataToWrite[1];
    dataToWrite[0] = reg;
    return ltc68xx_i2c_write_register(chain, addr, dataToWrite, sizeof(dataToWrite), successful, successfulSize);
}

bool ltc68xx_i2c_write_register8(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint8_t data, bool* successful, size_t successfulSize) {
    uint8_t dataToWrite[2];
    dataToWrite[0] = reg;
    dataToWrite[1] = data;

    return ltc68xx_i2c_write_register(chain, addr, dataToWrite, sizeof(dataToWrite), successful, successfulSize);
}

bool ltc68xx_i2c_write_register16(ltc68xx_chain_t* chain, uint8_t addr, uint8_t reg, uint16_t data, bool* successful, size_t successfulSize) {
    uint8_t dataToWrite[3];
    dataToWrite[0] = reg;
    dataToWrite[1] = data >> 8;
    dataToWrite[2] = data & 0xFF;

    return ltc68xx_i2c_write_register(chain, addr, dataToWrite, sizeof(dataToWrite), successful, successfulSize);
}

bool ltc68xx_i2c_write(ltc68xx_chain_t* chain, ltc68xx_i2c_data_t* data, size_t dataSize) {
    if (!ltc68xx_i2c_check(chain, data, dataSize))
        return false;

    int maxDataSize = ltc68xx_i2c_prepare_data(chain, data);

    // Generate I2C sequence
    int commIndex = 0;

    // We iterate over -1 to size
    // i == -1 is the start bit + the address
    for (int i = -1; i < maxDataSize; i++) {
        for (int j = 0; j < chain->numICs; j++) {
            ltc68xx_t* device = &chain->devices[j];
            ltc68xx_i2c_data_t* item = &data[j];

            if (i == -1) { // start + address
                uint8_t addr = item->addr << 1; // I2C address byte (write = no bit set at end)
                device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_START | LTC68XX_FCOMM_I2C_W_MASTER_NACK | (addr << 4);
                device->commNeedsAck[commIndex] = true;
            }
            else if (i < (int)item->dataSize - 1) { // data byte
                device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_BLANK | LTC68XX_FCOMM_I2C_W_MASTER_NACK | item->data[i] << 4;
                device->commNeedsAck[commIndex] = true;
            }
            else if (i == (int)item->dataSize - 1) { // data byte + stop
                device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_BLANK | LTC68XX_FCOMM_I2C_W_MASTER_NACK_STOP | item->data[i] << 4;
                device->commNeedsAck[commIndex] = false;
            }
            else { // transmit nothing
                device->commTx[commIndex] = LTC68XX_ICOMM_I2C_W_NO_TRANSMIT | LTC68XX_FCOMM_I2C_W_MASTER_NACK;
                device->commNeedsAck[commIndex] = false;
            }
        }

        commIndex++;

        // Pad communication registers by transmitting nothing (at the end)
        if (i == maxDataSize - 1)
            commIndex = ltc68xx_i2c_pad_comm(commIndex, chain);

        // Every 3 commands we need to write, start the communication and read the result
        if (commIndex >= LTC68XX_COMM_CMD_COUNT) {
            ltc68xx_perform_i2c_comm(chain);
            ltc68xx_i2c_check_and_copy_receive(i, chain, data, false);

            // Reset index
            commIndex = 0;
        }
    }
    return true;
}
