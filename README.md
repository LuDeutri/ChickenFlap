# ChickenFlap
This software is to control a chickenflap based on a stm32 processor and one OLED SSD1309 display with 6 buttons for the human interface and diffrent functions.
The main goal is a control unit which is connected to a DC motor to open and close a flap automatically to a setted time.
The case is 3d printed with an FDM Anycubic Cobra Plus printer

## Overview
![IMG_20231105_141743](https://github.com/LuDeutri/ChickenFlap/assets/56504337/6cc05d47-8a6c-4987-a504-3ab26a97609b)
![IMG_20231105_115016](https://github.com/LuDeutri/ChickenFlap/assets/56504337/79c37ca8-61ed-4f4a-9eb6-4f6166fc94a6)
![IMG_PCB](https://github.com/LuDeutri/ChickenFlap/assets/56504337/324a4fe2-9858-4b81-971b-3f886f15d37b)



**Buttons:**
- *Left side:* Button Left *(at the top)*, Button Back
- *Right side:* Button Right *(at the top)*, Button Enter
- *Bottom side:* Button Btn *(at the left)*, Button FlapCtrl
Left, Right, Back and Enter are used to navigate through the menu. More in the following.
Btn is used to start the starting animation during the operation. It is only possible if an RTC is used (defined in config.h)
FlapCtrl is used to open or close the flap directly. It can used to to stop an opening or closing flap. If the button is pressed again after stopping the actually moving, the flap is moving back.

At the top left of the case is the potentiometer which is used to set the motorspeed. It is only settable if the accordingly menu is selected on the LCD display. More for this below. Otherwise turning the potentiometer has no influence.

At the bottom left is the error led. If an error occured, the led lights up. In this case an error message is showing on the display and an ***ERROR*** marker is blinking in the top right of the display. The message can be reopen in the home screen with pressing the right button.
A prestage of some errors is implemented. In this case ***WARNING*** is shown in the top right of the display, and a warning description is shown.
The meaning of the error and warnings is readable in the error.c

Left from the error led is a window in the case for a lightsensor. If euqipped it can be used to open / close the flap depending of the day light. (Code is not yet implemented)


## Home Screen
![IMG_20231105_120543](https://github.com/LuDeutri/ChickenFlap/assets/56504337/057c9e58-b3bd-4736-ba86-054c187c7a12)

- **Timer status:** ON (Flap is opening and closing, OPENING (Flap is only opening), CLOSING (Flap is only closing), OFF (Automatically opening/closing deactivated)
- **Timer time:** Depending on Timer status the setted time to open/close the flap is showing
- **Flap:** Showing the actually status of the flap (Closed, Open, Closing, Opening)
- **Battery state of charge** (SOC) (top right, in percentage)
- **Room temperature** (in °C)
- **Battery voltage** (in Volt)
- Actually **clock time** (Below the middle line in 24h mode (adjustable))


## Menu Screen
![IMG_20231105_120559](https://github.com/LuDeutri/ChickenFlap/assets/56504337/da14a1b9-d316-44e8-bfdb-eb83e8ab05fd)

-**Watch:** On this page the actually clk time can be adjusted. The time is running on a RTC with a button cell battery. Its continuing even if the processor is restarting.
-**Timer:** Set the timer status and the timer time to open and close the flap.
-**Config:** Configuration menu page

## Watch Screen
![IMG_20231105_120604](https://github.com/LuDeutri/ChickenFlap/assets/56504337/8f554dce-aa15-4ff2-a2b1-3d0f6cfbf3f5)

With *Enter* is first the hour conter set able and after the second *Enter* the counter for the minutes. The adjustable Number is blinking. The counter for the seconds is reseted if Enter is pushed the third time to set the time.


## Timer Screen
![IMG_20231105_120610](https://github.com/LuDeutri/ChickenFlap/assets/56504337/c4adb8a5-16ea-49d5-87b2-296009c9122a)
![IMG_20231105_120621](https://github.com/LuDeutri/ChickenFlap/assets/56504337/4f75a294-dbc8-44bc-840f-25cf175d0a92)
![IMG_20231105_120629](https://github.com/LuDeutri/ChickenFlap/assets/56504337/7c562407-400f-4cbd-8027-3bf6d9e5b2ea)

With the button *Right* is moving through the diffrent settings. The chosen one is marked with an rectangle. With *Enter* is the marked one safed. The timer status in the top will be updated and showing the actually setting.


## Config Menu Screen
![IMG_20231105_120639](https://github.com/LuDeutri/ChickenFlap/assets/56504337/36808f35-6bbe-4bbe-a7b5-26ddad2e26a2)

The Config menu has 2 pages. On the first page the time the motor runs to open the flap is adjustable. For this there are 2 possibilitys which can be selected with button *Right* and *Enter*.
On the second page is the setting for the motorspeed selectable.

![IMG_20231105_120645](https://github.com/LuDeutri/ChickenFlap/assets/56504337/e76574e1-608a-48e9-af49-31d6beade8e8)
The First **"SetOnRun"** is to set the time during opening the flap. For that, the button *Flap_Ctrl* is used to start and stop the flap moving. The time is safed automatically.
If the Processor is restarting, the motor operation time is reseting to the default running time, which is settable in config.h. 

![IMG_20231105_120651](https://github.com/LuDeutri/ChickenFlap/assets/56504337/47b0b8e3-5733-40e9-af2c-70f40ebce3c9)
With the second one **"SetTime"** the time can be setted directly in seconds using button *Right* and *Enter*.

![IMG_20231105_120658](https://github.com/LuDeutri/ChickenFlap/assets/56504337/3aa61787-7a7d-4252-a876-b4858c2d143c)
The motorspeed is setted via an potentiometer which is mounted on the PCB and is showing in percentage from 5 - 100%. This setting should setted be first and only one time.
In addition it is important to use your brain. The setting reduce the duty cylce off the pwm low side switch and with that, the power supply voltage of the motor. If your VDD is to less and
the motorspeed setted to low, the voltage could be to less and the motor can not work corretly or damaged.


## Start Animation
https://github.com/LuDeutri/ChickenFlap/assets/56504337/b4e869bc-d05f-4472-a457-62c742082ffb
