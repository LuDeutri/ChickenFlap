#ifndef DART_COMMON_COMPAT_COMPAT2019_AMS_H_
#define DART_COMMON_COMPAT_COMPAT2019_AMS_H_

#include "../../../../chickenFlap/dart/standard_include.h"

#pragma pack(push, 1)
typedef struct {
    bool Error_Fault : 1;
    bool Controlled_Negative_Relay_State : 1;
    bool Controlled_Positive_Relay_State : 1;
    bool Controlled_Precharge_Relay_State : 1;
    bool Info_Driverless : 1;
    uint8_t Info_Vehicle : 3;
    uint8_t Info_SoftwareVersion : 8;
    uint8_t Info_HardwareVersion : 8;

    bool booting : 1;
    bool precharge : 1;
    bool drivemode : 1;
    bool contactorRequest : 1;
    bool SC_Delay_State : 1;
    bool AIR_Positive_Actual_State : 1;
    bool AIR_Negative_Actual_State : 1;
    bool Overall_HV_State : 1;

    bool Info_Disconnecting : 1;
    bool Info_Disconnected : 1;
    bool Warning_Overall  : 1;
    bool Warning_Unbalanced : 1;
    bool Warning_Cooldown : 1;
    bool Warning_PowerOverLimit : 1;
    bool Warning_SC_LowVoltage : 1;
    bool Error_Precharge_Timeout : 1;

    bool Error_Cell_Overvoltage  : 1;
    bool Error_Cell_Undervoltage : 1;
    bool Error_Cell_Overtemperature  : 1;
    bool Error_Cell_Undertemperature : 1;
    bool Error_Relay_Fault : 1;
    bool Error_ISOSPI : 1;
    bool Error_SDC_Undervoltage : 1;
    bool Error_Overcurrent  : 1;

    bool Error_Regen_Overcurrent : 1;
    bool Error_OverFuseIntegral : 1;
} CAN_Compat2019_AMS_Status_t;

typedef struct {
    uint16_t voltagePack;
    uint16_t cellMin;
    uint8_t cellMinID;
    uint16_t cellMax;
    uint8_t cellMaxID;
} CAN_Compat2019_AMS_Voltages_t;

typedef struct {
    uint16_t averageTemperature;
    uint8_t cellMin;
    uint8_t cellMinID;
    uint8_t cellMax;
    uint8_t cellMaxID;
    uint8_t fanPWM;
} CAN_Compat2019_AMS_Temperatures_t;

typedef struct {
    uint16_t busVoltage;
    uint16_t busCurrent;
    uint16_t sdcVoltage;
} CAN_Compat2019_AMS_EnergyMeter_t;

typedef struct {
    bool errorVCU : 1;
    bool errorIMD : 1;
    bool errorAMS : 1;
    bool resetSignal : 1;
    bool sdcClosed : 1;
    uint8_t : 3;
    uint8_t : 8;
    uint8_t : 8;
} CAN_Compat2019_AMS_SDC_Errors_t;

typedef struct {
    uint8_t id;
    uint16_t voltage;
    uint8_t temperature;
    bool balancing : 1;
    uint8_t : 7;
} CAN_Compat2019_AMS_LoopCellData_t;
#pragma pack(pop)

#define CAN_ID_COMPAT2019_AMS_SDC_ERRORS 0x200
#define CAN_ID_COMPAT2019_AMS_STATUS 0x300
#define CAN_ID_COMPAT2019_AMS_ENERGYMETER 0x301
#define CAN_ID_COMPAT2019_AMS_LOOP_CELL_DATA 0x302
#define CAN_ID_COMPAT2019_AMS_VOLTAGE 0x303
#define CAN_ID_COMPAT2019_AMS_TEMPERATURE 0x304


#endif
