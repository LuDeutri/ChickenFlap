#ifndef DART_COMMON_XI2020_AMS_H_
#define DART_COMMON_XI2020_AMS_H_

#include "../../../../chickenFlap/dart/standard_include.h"


#pragma pack(push, 1)
typedef struct {
    bool disconnecting : 1;
    bool connected : 1;
    bool precharging : 1;
    bool booting : 1;
    bool prechargeRelayTargetState : 1;
    bool positiveRelayTargetState : 1;
    bool negativeRelayTargetState : 1;
    bool errorState : 1;

    uint8_t softwareVersion : 4;
    uint8_t vehicle : 4;

    bool overallHVState : 1;
    bool negativeRelayActualState : 1;
    bool positiveRelayActualState : 1;
    bool sdcDelayState : 1;
    uint8_t hardwareVersion : 4;

    bool ready : 1;
    bool errorSlaveLTC : 1;
    bool errorInvalidCellVoltage : 1;
    bool contactorRequest : 1;
    bool errorLTCSelfTest : 1;
    bool errorLTCDigitalVoltage : 1;
    bool errorLTCAnalogVoltage : 1;
    bool errorLTCRefVoltage : 1;

    uint8_t : 2;
    bool warningSDCLowVoltage : 1;
    bool warningSDCardFail : 1;
    bool warningIMDError : 1;
    bool warningIMDPWMInvalid : 1;
    bool warningSlaveHumiditySensor : 1;
    bool warningOverall  : 1;

    bool errorChargeCurrentLimit  : 1;
    bool errorCurrentLimit : 1;
    bool errorSlaveCommunication : 1;
    bool errorRelayFault : 1;
    bool errorCellUndertemperature : 1;
    bool errorCellOvertemperature  : 1;
    bool errorCellUndervoltage : 1;
    bool errorCellOvervoltage  : 1;

    uint8_t : 1;
    bool errorEMCurrentTooHighForDisconnected : 1;
    bool errorEMCurrentInvalid : 1;
    bool errorEMVoltageInvalid : 1;
    bool errorEnergymeterCommunication : 1;
    bool errorTempSensorCommunication : 1;
    bool errorRelayDriverFault : 1;
    bool errorPrechargeTimeout : 1;
} CAN_AMS_Status_t;

typedef struct {
    uint16_t voltagePack;
    uint16_t minVoltage;

    uint8_t minModuleIndex : 4;
    uint8_t minCellIndex : 4;

    uint16_t maxVoltage;

    uint8_t maxModuleIndex : 4;
    uint8_t maxCellIndex : 4;
} CAN_AMS_Voltages_t;

typedef struct {
    uint8_t averageTemperature;
    uint8_t minTemperature;

    uint8_t minModuleIndex : 4;
    uint8_t minCellIndex : 4;

    uint8_t maxTemperature;

    uint8_t maxModuleIndex : 4;
    uint8_t maxCellIndex : 4;
    uint8_t fanPWM;
} CAN_AMS_Temperatures_t;

typedef struct {
    uint16_t busVoltage;
    uint16_t busCurrent;
    uint16_t sdcVoltage;
} CAN_AMS_EnergyMeter_t;

typedef struct {
    uint8_t : 3;
    bool sdcClosed : 1;
    bool resetSignal : 1;
    bool amsOpen : 1;
    bool imdOpen : 1;
    bool vcuOpen : 1;
} CAN_AMS_SDC_Status_t;

typedef struct {
    uint8_t id;
    uint16_t voltage;
    uint8_t temperature;

    uint8_t : 7;
    bool balancing : 1;
} CAN_AMS_LoopCell_t;

typedef struct {
    uint8_t stateOfCharge;
    uint16_t capacityLeft;
    uint16_t averageVoltage;
    uint16_t guessedOCV;
} CAN_AMS_StateOfCharge_t;

typedef struct {
    uint8_t slaveIndex;

    bool enabled : 1;
    bool ok : 1;
    bool errorI2C : 1;
    bool errorUnknownDevice : 1;
    uint8_t : 4;

    int16_t temperature;
    uint16_t humidity;
} CAN_AMS_Loop_Humidity;

typedef struct {
    uint8_t duty;
    uint8_t frequency;

    uint8_t : 1;
    bool connectionFaultEarth : 1;
    bool deviceError : 1;
    bool speedStart : 1;
    bool undervoltage : 1;
    bool normal : 1;
    bool unknownSignal : 1;
    bool diagnosticDisabled : 1;

    uint8_t faultResistance;

    uint8_t : 6;
    bool speedStartBad : 1;
    bool dutyInvalid : 1;
} CAN_AMS_IMD_t;

typedef struct {
    uint16_t discharge;
    uint16_t charge;
    uint8_t dischargeLimit;
    uint8_t chargeLimit;
} CAN_AMS_Fuseintegral_t;

typedef struct {
    bool airNegativeDiag : 1;
    bool airPositiveDiag : 1;
    bool led2 : 1;
    bool led1 : 1;
    bool amsOutput : 1;
    bool kNegative : 1;
    bool kPositive : 1;
    bool kPrecharge : 1;

    bool reset : 1;
    bool airNegative : 1;
    bool airPositive : 1;
    bool vcu : 1;
    bool amsHold : 1;
    bool imdHold : 1;
    bool sdcTimer250ms : 1;
    bool sdcReady : 1;

    uint8_t : 7;
    bool sdCardPresent : 1;

    uint8_t errorSlaveIndex;
    uint8_t errorCellIndex;
    uint8_t loopTime;
} CAN_AMS_Debug_t;

typedef struct {
    bool fanFixed : 1;
    uint8_t fanPercentage : 7;
    uint8_t minVoltage;
    uint8_t maxVoltage;
    uint8_t maxTemperature;
    bool enableBalancing : 1;
    bool balanceHighestCell : 1;
    bool forceUpdateOCV : 1;
    bool disableHumidity : 1;
    bool enableLTCDebug : 1;
    bool enableEMDebug : 1;
    bool testSlaveOrder : 1;
    bool testCellOrder : 1;
    bool dischargeAll : 1;
    bool dischargeModule : 1;
    uint8_t dischargeModuleIndex : 6;
} CAN_AMS_Set_Config_t;

typedef struct {
    int32_t voltageADC;
    int32_t currentADC;
} CAN_AMS_Debug_EM_t;

typedef struct {
    uint8_t slaveIndex;
    uint8_t cell1 : 4;
    uint8_t cell2 : 4;
    uint8_t cell3 : 4;
    uint8_t cell4 : 4;
    uint8_t cell5 : 4;
    uint8_t cell6 : 4;
    uint8_t cell7 : 4;
    uint8_t cell8 : 4;
    uint8_t cell9 : 4;
    uint8_t cell10 : 4;
    uint8_t cell11 : 4;
    uint8_t cell12 : 4;
    uint8_t pecErrorCounter;
} CAN_AMS_Debug_LTC_1_t;

typedef struct {
    uint8_t slaveIndex;
    uint8_t refVoltage;
    uint8_t analogVoltage;
    uint8_t digitalVoltage;
    bool openWireC0 : 1;
    bool openWireC1 : 1;
    bool openWireC2 : 1;
    bool openWireC3 : 1;
    bool openWireC4 : 1;
    bool openWireC5 : 1;
    bool openWireC6 : 1;
    bool openWireC7 : 1;
    bool openWireC8 : 1;
    bool openWireC9 : 1;
    bool openWireC10 : 1;
    bool openWireC11 : 1;
    bool openWireC12 : 1;
    uint8_t : 3;

    bool thermalShutdown : 1;
    bool selfTestMuxfail : 1;
    bool selfTestCellVoltagesFail : 1;
    bool selfTestStatusVoltagesFail: 1;
    bool selfTestAuxVoltagesFail : 1;
    bool selfTestOpenWireFail : 1;
    bool selfTestConfigFail : 1;
    uint8_t : 1;
    int8_t itemp;
} CAN_AMS_Debug_LTC_2_t;
#pragma pack(pop)

#define CAN_ID_AMS_STATUS 0x300
#define CAN_ID_AMS_ENERGYMETER 0x301
#define CAN_ID_AMS_LOOP_CELL_DATA 0x302
#define CAN_ID_AMS_VOLTAGE 0x303
#define CAN_ID_AMS_TEMPERATURE 0x304
#define CAN_ID_AMS_STATE_OF_CHARGE 0x305
#define CAN_ID_AMS_IMD 0x306
#define CAN_ID_AMS_FUSEINTEGRAL 0x307
#define CAN_ID_AMS_SDC_STATUS 0x308
#define CAN_ID_AMS_LOOP_HUMIDITY 0x309
#define CAN_ID_AMS_SET_CONFIG 0x310
#define CAN_ID_AMS_DEBUG 0x320
#define CAN_ID_AMS_DEBUG_LTC_1 0x321
#define CAN_ID_AMS_DEBUG_LTC_2 0x322
#define CAN_ID_AMS_DEBUG_EM 0x323
#endif
