Here a control unit for a chicken flap with an OLED 1309 display is implemented, based on one stm32f103.
Code is written in the stm32CubeIDE. 
The flap is controlled via 6 buttons, also a software watch is implemented, whereby a timer can be set for the
opening and/or optionally, closing of the chicken flap. 
The height which the flap must cover is variably adjustable.
In the config.h you can find most of the settings which are easy to adjust this way like the language,
error/warning timeouts, or diffrent flags to enable/disable diffrent functions.
