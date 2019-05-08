#include "powercontrol.h"


const static int relays[] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4, RELAY_PIN_5, RELAY_PIN_6};
void disableAir()
{
   PowerControlThread::instance()->setAirPump(false);
}

void disableWater()
{
   PowerControlThread::instance()->setWaterPump(false);
}

DECLARATE_INSTANCE(PowerControlThread)
PowerControlThread::PowerControlThread()
: 
   powerValue(0),
   disableAirPump(new Ticker),
   disableWaterPump(new Ticker)
{
   for ( auto i : relays )
      pinMode(i, OUTPUT);
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
      onRelay(CONVECTION_COOLER_2, HIGH);
      onRelay(CONVECTION_COOLER_1, HIGH);
      __globalState__.state.convection = CONVECTION_STATE_ON;
   }
   else
   {
      onRelay(CONVECTION_COOLER_1, LOW);
      onRelay(CONVECTION_COOLER_2, LOW);
      __globalState__.state.convection = CONVECTION_STATE_OFF;
   }
}

void PowerControlThread::setWaterPump(bool state)
{
   if ( state )
   {
      onRelay(WATER_PUMP, HIGH);
      __globalState__.state.water = WATER_PUMP_STATE_ON;
      disableWaterPump->once(180, disableWater);
   }
   else 
   {
      onRelay(WATER_PUMP, LOW);
      __globalState__.state.water = WATER_PUMP_STATE_OFF;
   }
}

void PowerControlThread::setAirPump(bool state)
{
   if ( state )
   {
      onRelay(AIR_PUMP, HIGH);
    __globalState__.state.air = AIR_PUMP_STATE_ON;
    disableAirPump->once(900, disableAir);
   }
   else
   {
      onRelay(AIR_PUMP, LOW);
    __globalState__.state.air = AIR_PUMP_STATE_OFF;
   }
}

void PowerControlThread::run()
{
   setHeating();
   if ( powerValue > 0 )
     __globalState__.state.heating_state = HEATING_STATE_ON;
   runned();
}