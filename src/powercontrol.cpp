#include "powercontrol.h"



void disableWater()
{
   PowerControlThread::instance()->setWaterPump(false);
}

void disableIgnition()
{
   PowerControlThread::instance()->setIgnition(false);
}

DECLARATE_INSTANCE(PowerControlThread)
PowerControlThread::PowerControlThread()
: 
   powerValue(0),
   disableWaterPump(new Ticker),
   disableIgnitionModule(new Ticker)
{
  
}

void PowerControlThread::onRelay(int relayPin, int value)
{
   switch(relayPin)
   {
      case TEH_BOTTOM: 
      case TEH_TOP:
         analogWrite(relayPin, value);
         break;
      default:
         digitalWrite(relayPin, value);
         break;
   }
}

void PowerControlThread::setHeating()
{
    onRelay(TEH_TOP, powerValue);
    onRelay(TEH_BOTTOM, powerValue);
}

void PowerControlThread::setConvection(bool state)
{
   if ( state )
   {
      onRelay(CONVECTION_COOLER_1, HIGH);
   }
   else
   {
      onRelay(CONVECTION_COOLER_1, LOW);
   }
   GlobalState::instance()->setConvectionState(state);
}

void PowerControlThread::setWaterPump(bool state)
{
   if ( state )
   {
      onRelay(WATER_PUMP, HIGH);
      disableWaterPump->once(180, disableWater);
   }
   else 
   {
      onRelay(WATER_PUMP, LOW);
   }
   GlobalState::instance()->setWaterPumpState(state);
}

void PowerControlThread::setAirPump(bool state)
{
   if ( state )
   {
      onRelay(AIR_PUMP, HIGH);
   }
   else
   {
      onRelay(AIR_PUMP, LOW);
   }
   GlobalState::instance()->setAirPumpState(state);
}

void PowerControlThread::setIgnition(bool state)
{
   if ( state )
   {
      onRelay(IGNITION, HIGH);
      disableIgnitionModule->once(30, disableIgnition);
   }
   else
   {
      onRelay(IGNITION, LOW);
   }
   GlobalState::instance()->setIgnitionState(state);
}

void PowerControlThread::run()
{
   setHeating();
   if ( powerValue > 0 )
     GlobalState::instance()->setHeatingState(true);
   runned();
}