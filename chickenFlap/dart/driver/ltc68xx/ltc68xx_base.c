#include "../../../../chickenFlap/dart/driver/ltc68xx/ltc68xx_base.h"

uint8_t ltc68xx_convert_adc_mode(ltc68xx_adc_mode_t mode) {
    switch(mode) {
        case LTC68XX_ADC_422Hz:
        case LTC68XX_ADC_1kHz:
            return 0;
        case LTC68XX_ADC_14kHz:
        case LTC68XX_ADC_27kHz:
            return 1;
        default:
        case LTC68XX_ADC_3kHz:
        case LTC68XX_ADC_7kHz:
            return 2;
        case LTC68XX_ADC_26Hz:
        case LTC68XX_ADC_2kHz:
            return 3;
    }
}

util_time_t ltc68xx_get_adc_time(ltc68xx_chain_t* chain) {
    DART_NOT_NULL_RETURN(chain, DART_ERROR_INVALID_VALUE, 202);

    switch(chain->adc_mode) {
        case LTC68XX_ADC_27kHz:
        case LTC68XX_ADC_14kHz:
        case LTC68XX_ADC_7kHz:
            return 3;
        case LTC68XX_ADC_3kHz:
        case LTC68XX_ADC_2kHz:
            return 5;
        case LTC68XX_ADC_1kHz:
            return 11;
        case LTC68XX_ADC_422Hz:
            return 12;
        default:
        case LTC68XX_ADC_26Hz:
            return 202;
    }
}

int ltc68xx_get_gpio_count(const ltc68xx_t device) {
    switch(device.type) {
        case LTC6804:
            return 5;
        case LTC6810:
            return 4;
        case LTC6811:
            return 5;
        case LTC6813:
            return 9;
        default:
            DART_ERROR_RETURN(DART_ERROR_INVALID_VALUE, 0);
    }
}

int ltc68xx_get_max_cell_count(const ltc68xx_t device) {
    switch(device.type) {
        case LTC6804:
            return 12;
        case LTC6810:
            return 6;
        case LTC6811:
            return 12;
        case LTC6813:
            return 18;
        default:
            DART_ERROR_RETURN(DART_ERROR_INVALID_VALUE, 0);
    }
}
