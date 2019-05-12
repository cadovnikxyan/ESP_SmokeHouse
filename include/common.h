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
#define RELAY_PIN_7   D7 // IGNITION
#define RELAY_COUNT   7

#define TEH_TOP             RELAY_PIN_1
#define TEH_BOTTOM          RELAY_PIN_2
#define WATER_PUMP          RELAY_PIN_3
#define AIR_PUMP            RELAY_PIN_4
#define CONVECTION_COOLER_1 RELAY_PIN_5
#define CONVECTION_COOLER_2 RELAY_PIN_6
#define IGNITION            RELAY_PIN_7

#define CS            D8 //NOT_USED

#define B 3899
#define SERIAL_R 100000
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

#define MANUAL_MODE              0x1000000
#define AUTO_MODE                0x2000000
#define NO_HEATING               0x3000000
#define SMOKING_MODE             0x4000000

#define HEATING_STATE_ON         0x0100000
#define HEATING_STATE_OFF        0x0200000

#define HEATING_NONE_STATE       0x0010000
#define HEATING_DRYING_STATE     0x0020000
#define HEATING_FRYING_STATE     0x0030000
#define HEATING_BOILING_STATE    0x0040000

#define CONVECTION_STATE_ON      0x0001000
#define CONVECTION_STATE_OFF     0x0002000

#define AIR_PUMP_STATE_ON        0x0000100
#define AIR_PUMP_STATE_OFF       0x0000200

#define WATER_PUMP_STATE_ON      0x0000010
#define WATER_PUMP_STATE_OFF     0x0000020

#define IGNITION_MODULE_STATE_ON    0x0000001
#define IGNITION_MODULE_STATE_OFF    0x0000002

struct State{
    
    int mode;
    int heating_state;
    int heating_mode;
    int convection;
    int air;
    int water;
    int ignition;
    State(){
        mode =  MANUAL_MODE;
        heating_state =  HEATING_STATE_OFF;
        heating_mode =  HEATING_NONE_STATE;
        convection =  CONVECTION_STATE_OFF;
        air =  AIR_PUMP_STATE_OFF;
        water =  WATER_PUMP_STATE_OFF;
        ignition = IGNITION_MODULE_STATE_OFF;
    }
    operator int(){
        int state = 0x0000000;
        state |=  mode;
        state |=  heating_mode;
        state |=  heating_state;
        state |=  convection;
        state |=  air;
        state |=  water;
        state |=  ignition;
        return state;
    }
};

struct GlobalState
{
    DEFINE_INSTANCE(GlobalState)
    double currentOutTemp;
    double currentProbeTemp;
    State state;
    private:
        GlobalState(const GlobalState& other)
        {
            state = other.state;
            currentOutTemp = other.currentOutTemp;
            currentProbeTemp = other.currentProbeTemp;
        }
        GlobalState& operator=(const GlobalState& other)
        {
            state = other.state;
            currentOutTemp = other.currentOutTemp;
            currentProbeTemp = other.currentProbeTemp;
            return *this;
        }

    public:
    String getMode()
    {
        String modeValue;
        if ( state.mode == MANUAL_MODE )
            modeValue = "manual";
        else if (state.mode == AUTO_MODE )
            modeValue = "auto";
        else if (state.mode == NO_HEATING )
            modeValue = "no_heating";
        else if (state.mode == SMOKING_MODE )
            modeValue = "smoking";
        return modeValue;
    }

    bool setMode(String mode)
    {
        if ( mode == "manual" )
            state.mode = MANUAL_MODE;
        else if (mode == "auto" )
            state.mode = AUTO_MODE;
        else if (mode ==  "no_heating")
            state.mode = NO_HEATING;
        else if (mode == "smoking")
            state.mode = SMOKING_MODE;
        else
            return false;
        return true;
    }

    bool getHeatingState()
    {
        return ( state.heating_state == HEATING_STATE_ON );
    }

    void setHeatingState(bool heating)
    {
        if ( heating )
            state.heating_state = HEATING_STATE_ON;
        else
            state.heating_state = HEATING_STATE_OFF;
    }

    String getHeatingMode()
    {
        String heatingMode;
        if ( state.heating_mode == HEATING_NONE_STATE )
            heatingMode = "none";
        else if ( state.heating_mode == HEATING_DRYING_STATE )
            heatingMode = "drying";
        else if ( state.heating_mode == HEATING_FRYING_STATE )
            heatingMode = "frying";
        else if ( state.heating_mode == HEATING_BOILING_STATE )
            heatingMode = "boiling";
        return heatingMode;
    }

    bool setHeatingMode(String mode)
    {
        if ( mode ==  "none")
            state.heating_mode = HEATING_NONE_STATE;
        else if ( mode == "drying" )
            state.heating_mode = HEATING_DRYING_STATE;
        else if ( mode == "frying" )
            state.heating_mode = HEATING_FRYING_STATE;
        else if ( mode == "boiling" )
            state.heating_mode = HEATING_BOILING_STATE;
        else 
            return false;
            
        return true;
    }

    bool getConvectionState()
    {
        return ( state.convection == CONVECTION_STATE_ON );
    }

    void setConvectionState(bool convection)
    {
        if ( convection )
            state.convection = CONVECTION_STATE_ON;
        else
            state.convection = CONVECTION_STATE_OFF;
    }

    bool getAirPumpState()
    {
        return ( state.air == AIR_PUMP_STATE_ON );
    }

    void setAirPumpState(bool airState)
    {
        if ( airState )
            state.air = AIR_PUMP_STATE_ON;
        else
            state.air = AIR_PUMP_STATE_OFF;
    }

    bool getWaterPumpState()
    {
        return ( state.water == WATER_PUMP_STATE_ON );
    }

    void setWaterPumpState(bool waterState)
    {
        if ( waterState )
            state.water = WATER_PUMP_STATE_ON;
        else
            state.water = WATER_PUMP_STATE_OFF;
    }

    bool getIgnitionState()
    {
        return ( state.ignition == IGNITION_MODULE_STATE_ON );
    }

    void setIgnitionState(bool ignitionState)
    {
        if ( ignitionState )
            state.ignition = IGNITION_MODULE_STATE_ON;
        else
            state.ignition = IGNITION_MODULE_STATE_OFF;
    }
};


#endif