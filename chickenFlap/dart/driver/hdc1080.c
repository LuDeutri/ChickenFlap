#include "../../../chickenFlap/dart/driver/hdc1080.h"

float hdc1080_convert_temperature(uint16_t value) {
    return (from_motorola_uint16(value) / 65536.0f) * 165.0f - 40.0f;
}

float hdc1080_convert_humidity(uint16_t value) {
    return from_motorola_uint16(value) / 65536.0f;
}
