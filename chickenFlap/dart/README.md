# DART Hardware abstraction layer
Hardware abstraction layer for every processor we use. This source code should be checked out in the subfolder called `dart`.
The compiliation is controlled over build defines.

## Example:

Declare global define `USE_BOARD_STM32F103_DEVBOARD`.

```
#include "dart/hal_wrapper/hal_wrapper.h"
#include "dart/util/time.h"

void test_main() {
	// Initialize HAL
	board_init();
	hal_init();

	// Example code
	digitalMode(DIGITAL_LED_GREEN, ModeOutput);

	while(1) {
		digitalToggle(DIGITAL_LED_GREEN);
		delayMillis(1000);
	}
}

void dart_error(int errorCode) {

}
```
