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
   root["tempProbe"] = tempValue;
   String result;
   serializeJson(root, result);
   return result;
}

double ProbeThread::getTemp()
{
   int t = analogRead( THERMISTORPIN );
   double tr = 1023.0 / t - 1;
   tr = SERIAL_R / tr;

   double steinhart;
   steinhart = tr / THERMISTOR_R; // (R/Ro)
   steinhart = log(steinhart); // ln(R/Ro)
   steinhart /= B; // 1/B * ln(R/Ro)
   steinhart += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
   steinhart = 1.0 / steinhart; // Invert
   steinhart -= 273.15;
   return steinhart;
}

void ProbeThread::run()
{
   tempValue = getTemp();
   delay(100);
   runned();
}