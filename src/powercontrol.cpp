#include "powercontrol.h"

const static int relays[] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4, RELAY_PIN_5, RELAY_PIN_6};

DECLARATE_INSTANCE(PowerControlThread)
PowerControlThread::PowerControlThread()
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
      default:
         digitalWrite(relayPin, value);
   }
}

void PowerControlThread::setHeating(bool state)
{
   if ( state )
   {
      onRelay(TEH_TOP, powerValue);
      onRelay(TEH_BOTTOM, powerValue);
   }
   else
   {
      onRelay(TEH_TOP, LOW);
      onRelay(TEH_BOTTOM, LOW);
   }
}

void PowerControlThread::setConvection(bool state)
{
   if ( state )
   {
      onRelay(CONVECTION_COOLER_2, HIGH);
      onRelay(CONVECTION_COOLER_1, HIGH);
   }
   else
   {
      onRelay(CONVECTION_COOLER_1, LOW);
      onRelay(CONVECTION_COOLER_2, LOW);
   }
}

void PowerControlThread::setWaterPump(bool state)
{
   if ( state )
      onRelay(WATER_PUMP, HIGH);
   else 
      onRelay(WATER_PUMP, LOW);
}

void PowerControlThread::setAirPump(bool state)
{
   if ( state )
      onRelay(AIR_PUMP, HIGH);
   else
      onRelay(AIR_PUMP, LOW);
}

void PowerControlThread::setMode(const String& mode)
{
   if ( mode == "HEATING_MANUALLY")
      currentMode = MANUAL_MODE;
   else if ( mode == "HEATING_AUTO")
      currentMode = AUTO_MODE;
   else if ( mode == "NO_HEATING")
      currentMode =  NO_HEATING;
   else if (mode == "SMOKING")
      currentMode = SMOKING_MODE;
   else
   {
      currentMode = 0x00;
   }
}

uint8_t PowerControlThread::getCurrentMode() const
{
    return currentMode;
}

void PowerControlThread::run()
{
   setHeating(true);
   runned();
}