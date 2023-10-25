#include "../../../../chickenFlap/dart/hal_wrapper/tms570/spi.h"

#if defined(USE_TMS570) && defined(HAL_ENABLE_SPI)
spiBASE_t* spiMap[SPI_COUNT] = SPI_MAP;

uint16_t spi_txBuffer[256];
uint16_t spi_rxBuffer[256];

void hal_spi_init() {
    spiInit();
}

void hal_spi_update() {
    // Do nothing
}

bool hal_spi_begin(int spi) {
    DART_NOT_USED(spi);
    // Do nothing
    return true;
}

bool hal_spi_end(int spi) {
    DART_NOT_USED(spi);
    // Do nothing
    return true;
}

bool hal_spi_write(int spi, uint8_t* data, size_t size) {
    spiBASE_t* handle = spiMap[spi];
    DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(size <= sizeof(spi_txBuffer), DART_ERROR_BUFFER_OVERFLOW, false);

    // Default settings, CS pin needs to be controlled by GPIO functions
    // SPI_FMT_0 should be char length 8 bits
    spiDAT1_t config;
    config.CSNR = SPI_CS_0;
    config.WDEL = false;
    config.CS_HOLD = true;
    config.DFSEL = SPI_FMT_0;

    for (int i = 0; i < size; i++)
        spi_txBuffer[i] = data[i];
    if (spiTransmitData(handle, &config, size, spi_txBuffer) != 0)
        return false;
    return true;
}

bool hal_spi_read(int spi, uint8_t* data, size_t size) {
    spiBASE_t* handle = spiMap[spi];
    DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(size <= sizeof(spi_rxBuffer), DART_ERROR_BUFFER_OVERFLOW, false);

    // Default settings, CS pin needs to be controlled by GPIO functions
    // SPI_FMT_0 should be char length 8 bits
    spiDAT1_t config;
    config.CSNR = SPI_CS_0;
    config.WDEL = false;
    config.CS_HOLD = true;
    config.DFSEL = SPI_FMT_0;

    if (spiReceiveData(handle, &config, size, spi_rxBuffer) != 0)
        return false;
    for (int i = 0; i < size; i++)
        data[i] = (spi_rxBuffer[i] & 0xFF);
    return true;
}

bool hal_spi_transfer(int spi, uint8_t* tx, uint8_t* rx, size_t size) {
    spiBASE_t* handle = spiMap[spi];
    DART_NOT_NULL_RETURN(handle, DART_ERROR_INVALID_VALUE, false);
    DART_ASSERT_RETURN(size <= sizeof(spi_rxBuffer), DART_ERROR_BUFFER_OVERFLOW, false);
    DART_ASSERT_RETURN(size <= sizeof(spi_txBuffer), DART_ERROR_BUFFER_OVERFLOW, false);

    // Default settings, CS pin needs to be controlled by GPIO functions
    // SPI_FMT_0 should be char length 8 bits
    spiDAT1_t config;
    config.CSNR = SPI_CS_0;
    config.WDEL = false;
    config.CS_HOLD = true;
    config.DFSEL = SPI_FMT_0;

    for (int i = 0; i < size; i++)
        spi_txBuffer[i] = tx[i];
    if (spiTransmitAndReceiveData(handle, &config, size, spi_txBuffer, spi_rxBuffer) != 0)
        return false;
    for (int i = 0; i < size; i++)
        rx[i] = (spi_rxBuffer[i] & 0xFF);
    return true;
}

bool hal_spi_set_speed(int spi, int frequency) {
    // Do nothing
    return false;
}
#endif
