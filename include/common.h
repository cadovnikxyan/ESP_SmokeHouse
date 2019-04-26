#ifndef COMMON_H
#define COMMON_H

#include <ArduinoJson.h>
#include "Thread.h"
#include "ThreadController.h"
#include <Ticker.h>
#include <time.h>

#define DEFINE_INSTANCE(Class) private:\
                               Class();\
                               public: \
                               static Class* instance();
                               
#define DECLARATE_INSTANCE(Class)  static Class* static##Class##Ptr = nullptr;\
                                   Class* Class::instance(){\
                                       if ( static##Class##Ptr == nullptr )\
                                          {\
                                            static##Class##Ptr = new Class();\
                                          }\
                                       return static##Class##Ptr;\
                                    }    

#define THERMISTORPIN A0 
#define TEMP_PIN      D8
#define RELAY_PIN_1   D1 // TEH_TOP
#define RELAY_PIN_2   D2 // TEH_BOTTOM
#define RELAY_PIN_3   D3 // WATER_PUMP
#define RELAY_PIN_4   D4 // AIR_PUMP
#define RELAY_PIN_5   D5 // CONVECTION_COOLER_1
#define RELAY_PIN_6   D6 // CONVECTION_COOLER_2
#define RELAY_COUNT   6

#define TEH_TOP             RELAY_PIN_1
#define TEH_BOTTOM          RELAY_PIN_2
#define WATER_PUMP          RELAY_PIN_3
#define AIR_PUMP            RELAY_PIN_4
#define CONVECTION_COOLER_1 RELAY_PIN_5
#define CONVECTION_COOLER_2 RELAY_PIN_6

#define SO            D7 //NOT_USED
#define CS            D8 //NOT_USED

#define B 3899
#define SERIAL_R 10000
#define THERMISTOR_R 200000
#define NOMINAL_T 25

#define DRYING_DONE_TEMP 40
#define FRYING_DONE_TEMP 60
#define FULL_DONE_TEMP 71

#define DRYING_OUT_TEMP 60
#define FRYING_OUT_TEMP 85
#define BOILING_OUT_TEMP 80

#define OUT_MAX_VALUE_PID 853

/*
State field description : 
 0x012345

 0: Mode 
 1: Heating (on/off)
 2: Heating state
 3: Convection state (on/off)
 4: Smoking state (air pump on/off)
 5: Water pump state (on/off)

*/

#define MANUAL_MODE              0x100000
#define AUTO_MODE                0x200000
#define NO_HEATING               0x300000
#define SMOKING_MODE             0x400000

#define HEATING_STATE_ON         0x010000
#define HEATING_STATE_OFF        0x020000

#define HEATING_NONE_STATE       0x001000
#define HEATING_DRYING_STATE     0x002000
#define HEATING_FRYING_STATE     0x003000
#define HEATING_BOILING_STATE    0x004000

#define CONVECTION_STATE_ON      0x000100
#define CONVECTION_STATE_OFF     0x000200

#define AIR_PUMP_STATE_ON        0x000010
#define AIR_PUMP_STATE_OFF       0x000020

#define WATER_PUMP_STATE_ON      0x000001
#define WATER_PUMP_STATE_OFF     0x000002

struct GlobalState
{
    double currentOutTemp;
    double currentProbeTemp;
    int state;
    GlobalState():
    state(0x000000)
    {
        state |=  MANUAL_MODE;
        state |=  HEATING_STATE_OFF;
        state |=  HEATING_NONE_STATE;
        state |=  CONVECTION_STATE_OFF;
        state |=  AIR_PUMP_STATE_OFF;
        state |=  WATER_PUMP_STATE_OFF;
    }

    String getMode()
    {
        String modeValue;
        if ( state & MANUAL_MODE )
            modeValue = "manual";
        else if (state & AUTO_MODE )
            modeValue = "auto";
        else if (state & NO_HEATING )
            modeValue = "no_heating";
        else if (state & SMOKING_MODE )
            modeValue = "smoking";
        return modeValue;
    }

    String getHeatingState()
    {
        String heatingState;
        if ( state & HEATING_STATE_ON )
            heatingState = "on";
        else if (state & HEATING_STATE_OFF )
            heatingState = "off";
        return heatingState;
    }

    String getHeatingMode()
    {
        String heatingMode;
        if ( state & HEATING_NONE_STATE )
            heatingMode = "none";
        else if ( state & HEATING_DRYING_STATE )
            heatingMode = "drying";
        else if (state & HEATING_FRYING_STATE )
            heatingMode = "frying";
        else if (state & HEATING_BOILING_STATE )
            heatingMode = "boiling";
        return heatingMode;
    }

    String getConvectionState()
    {
        String convectionState;
        if ( state & CONVECTION_STATE_ON )
            convectionState = "on";
        else if (state & CONVECTION_STATE_OFF )
            convectionState = "off";
        return convectionState;
    }

    String getAirPumpState()
    {
        String airPumpState;
        if ( state & AIR_PUMP_STATE_ON )
            airPumpState = "on";
        else if (state & AIR_PUMP_STATE_OFF )
            airPumpState = "off";
        return airPumpState;
    }

    String getWaterPumpState()
    {
        String waterPumpState;
        if ( state & WATER_PUMP_STATE_ON )
            waterPumpState = "on";
        else if (state & WATER_PUMP_STATE_OFF )
            waterPumpState = "off";
        return waterPumpState;
    }
};

static GlobalState __globalState__;

#endif