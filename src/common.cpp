#include "common.h"

DECLARATE_INSTANCE(GlobalState)
GlobalState::GlobalState():
    currentOutTemp(0),
    currentProbeTemp(0)
    {
    }

int GlobalState::getMode()
{
    return state.mode;
}

bool GlobalState::setMode(int mode)
{

    if ( mode == MANUAL_MODE || mode == AUTO_MODE || mode == SMOKING_MODE)
    {
        state.mode = mode;
        return true;
    }
    else 
        return false;
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

int GlobalState::getHeatingMode()
{
    return state.heating_mode;
}

bool GlobalState::setHeatingMode(int mode)
{
    if ( mode == HEATING_NONE_STATE || mode == HEATING_DRYING_STATE || mode == HEATING_FRYING_STATE || mode == HEATING_BOILING_STATE)
    {
        state.heating_mode = mode;
        return true;
    }
    else 
        return false;
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