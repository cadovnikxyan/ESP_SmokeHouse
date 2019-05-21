#include "probetemp.h"

DECLARATE_INSTANCE(ProbeThread)

ProbeThread::ProbeThread()
{
   pinMode( THERMISTORPIN, INPUT );
}

String ProbeThread::getJsonProbeTemp()
{
   StaticJsonDocument<200> jsonBuffer;
   JsonObject root = jsonBuffer.to<JsonObject>();
   tempValue = getTemp();
   root["tempProbe"] = tempValue;
   Serial.println(tempValue);
   String result;
   serializeJson(root, result);
   return result;
}

double ProbeThread::getTemp()
{
   std::vector<int> average;
   for ( int i = 0 ; i < 10; ++ i )
   {
      average.push_back(analogRead( THERMISTORPIN ));
      delay(10);
   }
   int t = std::accumulate(average.begin(), average.end(), 0) / average.size();

   double Rth = 1023.0 / t -1;
   Rth = SERIAL_R / Rth;
   double temperature;
   temperature = Rth / THERMISTOR_R; // (R/Ro)
   temperature = log(temperature); // ln(R/Ro)
   temperature /= B; // 1/B * ln(R/Ro)
   temperature += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
   temperature = 1.0 / temperature; // Invert
   temperature -= 273.15;
    
   Serial.println(Rth);
   Serial.println(temperature);
   if ( temperature > 200 || temperature < 0 )
      return 0;
   if ( temperature <= DRYING_DONE_TEMP )
      GlobalState::instance()->state.heating_mode = HEATING_DRYING_STATE;
   else if ( temperature >= DRYING_DONE_TEMP && temperature < FRYING_DONE_TEMP )
      GlobalState::instance()->state.heating_mode = HEATING_FRYING_STATE;
   else if ( temperature >= FRYING_DONE_TEMP && temperature < FULL_DONE_TEMP )
      GlobalState::instance()->state.heating_mode = HEATING_BOILING_STATE;
   else 
      GlobalState::instance()->state.heating_mode = HEATING_NONE_STATE;

   return temperature;
}

void ProbeThread::run()
{
   tempValue = getTemp();
   GlobalState::instance()->currentProbeTemp = tempValue;
   delay(100);
   runned();
}