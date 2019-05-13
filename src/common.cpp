#include "common.h"

DECLARATE_INSTANCE(GlobalState)
GlobalState::GlobalState():
    currentOutTemp(0),
    currentProbeTemp(0)
    {
    }

String GlobalState::getMode()
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

bool GlobalState::setMode(String mode)
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

bool GlobalState::getHeatingState()
{
    return ( state.heating_state == HEATING_STATE_ON );
}

void GlobalState::setHeatingState(bool heating)
{
    if ( heating )
        state.heating_state = HEATING_STATE_ON;
    else
        state.heating_state = HEATING_STATE_OFF;
}

String GlobalState::getHeatingMode()
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

bool GlobalState::setHeatingMode(String mode)
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

bool GlobalState::getConvectionState()
{
    return ( state.convection == CONVECTION_STATE_ON );
}

void GlobalState::setConvectionState(bool convection)
{
    if ( convection )
        state.convection = CONVECTION_STATE_ON;
    else
        state.convection = CONVECTION_STATE_OFF;
}

bool GlobalState::getAirPumpState()
{
    return ( state.air == AIR_PUMP_STATE_ON );
}

void GlobalState::setAirPumpState(bool airState)
{
    if ( airState )
        state.air = AIR_PUMP_STATE_ON;
    else
        state.air = AIR_PUMP_STATE_OFF;
}

bool GlobalState::getWaterPumpState()
{
    return ( state.water == WATER_PUMP_STATE_ON );
}

void GlobalState::setWaterPumpState(bool waterState)
{
    if ( waterState )
        state.water = WATER_PUMP_STATE_ON;
    else
        state.water = WATER_PUMP_STATE_OFF;
}

bool GlobalState::getIgnitionState()
{
    return ( state.ignition == IGNITION_MODULE_STATE_ON );
}

void GlobalState::setIgnitionState(bool ignitionState)
{
    if ( ignitionState )
        state.ignition = IGNITION_MODULE_STATE_ON;
    else
        state.ignition = IGNITION_MODULE_STATE_OFF;
}