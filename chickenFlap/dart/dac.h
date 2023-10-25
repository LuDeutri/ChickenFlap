#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

void dartDAC_init();
void dartDAC_update();
bool dacWrite(uint8_t channel, uint32_t data);

#ifdef __cplusplus
}
#endif
