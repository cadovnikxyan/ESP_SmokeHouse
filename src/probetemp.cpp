#include "probetemp.h"

#include <map>
#include <algorithm>

#include "dallastemp.h"
DECLARATE_INSTANCE(ProbeThread)

ProbeThread::ProbeThread()
{
   pinMode( THERMISTORPIN, INPUT );
}

std::vector<double> valuesTemp = {
 29.81, 31.50, 32.94, 33.92, 34.38,
 35.23, 35.65, 35.77, 36.02, 36.13,
 36.19, 36.33, 36.42, 36.63, 36.77,
 37.04, 37.23, 37.35, 37.50, 37.56,
 37.75, 37.96, 38.06, 38.15, 38.25,
 38.50, 38.54, 38.69, 38.85, 39.19,
 39.48, 39.54, 39.69, 39.83, 39.94,
 39.98, 40.17, 40.31, 40.46, 40.60,
 40.75, 40.90, 41.08, 41.19, 41.29,
 41.38, 41.44, 41.58, 41.73, 41.85,
 41.96, 42.19, 42.33, 42.48, 42.58,
 42.81, 42.98, 43.13, 43.31, 43.40,
 43.54, 43.63, 43.75, 44.00, 44.08,
 44.21, 44.42, 44.60, 44.71, 44.98,
 45.08, 45.27, 45.48, 45.60, 45.69,
 45.94, 46.00, 46.27, 46.38, 46.46,
 46.69, 46.85, 47.00, 47.17, 47.38,
 47.48, 47.58, 47.65, 47.77, 47.98,
 48.13, 48.33, 48.52, 48.73, 48.94,
 22.23, 22.44, 49.50, 49.60, 49.67,
 49.90, 50.15, 50.29, 50.42, 50.65,
 50.85, 51.06, 51.27, 51.35, 22.56,
 51.56, 51.69, 51.83, 52.00, 52.31,
 23.90, 52.48, 52.60, 52.81, 53.02,
 53.15, 53.38, 53.50, 53.75, 53.83,
 53.92, 54.06, 54.31, 54.50, 54.60,
 54.69, 54.81, 54.92, 55.25, 55.38,
 55.42, 55.52, 55.81, 56.06, 56.19,
 56.42, 56.60, 56.69, 56.90, 57.13,
 57.27, 57.52, 57.69, 57.92, 58.08,
 58.13, 58.38, 58.54, 58.65, 58.77,
 58.81, 59.08, 59.27, 59.54, 59.71,
 59.90, 59.96, 60.17, 37.31, 60.69,
 60.81, 61.00, 61.10, 61.38, 61.56,
 61.65, 61.96, 62.19, 62.44, 62.67,
 62.94, 62.96, 63.10, 63.29, 63.42,
 38.69, 63.71, 63.94, 64.10, 64.40,
 64.52, 64.85, 65.08, 65.10, 65.21,
 65.33, 65.65, 65.85, 66.00, 66.21,
 66.65, 66.83, 66.96, 67.02, 67.25,
 67.50, 67.79, 68.06, 68.31, 68.48,
 68.62, 68.90, 69.06, 69.25, 69.56,
 69.90, 69.92, 70.02, 70.37, 70.69,
 70.87, 71.00, 71.27, 71.31, 71.65,
 49.77, 72.37, 72.71, 72.77, 73.00,
 73.10, 73.17, 73.29};

std::vector<int> adcValues = {
397,406,413,412,410,
404,403,402,401,400,
399,398,396,394,393,
391,390,389,388,387,
386,385,384,382,381,
380,379,378,377,375,
374,373,372,370,369,
368,367,366,365,364,
363,362,361,360,359,
358,357,356,355,354,
353,352,351,350,349,
348,346,345,344,343,
342,341,340,339,338,
337,336,334,333,331,
330,329,328,327,326,
325,324,322,321,320,
319,318,317,316,315,
314,313,312,310,309,
308,307,306,305,303,
302,301,300,299,297,
296,295,293,292,291,
290,289,288,287,286,
285,283,282,281,280,
279,278,277,276,274,
273,272,271,270,269,
268,267,266,265,264,
263,262,261,260,259,
258,257,256,255,254,
253,252,250,248,247,
246,245,244,243,242,
241,240,239,238,237,
236,235,234,233,232,
231,230,229,228,226,
225,224,223,222,221,
220,219,218,217,216,
215,214,213,212,211,
210,209,208,207,206,
205,204,203,202,201,
200,199,198,197,196,
195,194,193,192,190,
189,188,187,186,185,
184,183,182,181,180,
179,178,177,176,175,
174,173,172,171,170,
169,168,167,166,165,
164,163,162
};

double ProbeThread::getPredefinedValue(int adc)
{
   auto it = std::find(adcValues.begin(), adcValues.end(), adc);   
   int index = std::distance(adcValues.begin(), it); 	
   if ( index < valuesTemp.size())
      return valuesTemp.at(index);
   return -1;
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
   if ( t < 162 || t > 412)
   {
      temperature = Rth / THERMISTOR_R; // (R/Ro)
      temperature = log(temperature); // ln(R/Ro)
      temperature /= B; // 1/B * ln(R/Ro)
      temperature += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
      temperature = 1.0 / temperature; // Invert
      temperature -= 273.15;
   }
   else
   {
      temperature = getPredefinedValue(t);
   }
   
   
   // Serial.print("adc value: ");
   // Serial.print(t); 
   // Serial.print("; resistance value: ");
   // Serial.print(Rth);
   // Serial.print("; temp value: ");
   // Serial.print(temperature);
   // Serial.print("; dallas temp: ");
   // DallasTempThread::instance()->getJsonTemp();
   // Serial.println(GlobalState::instance()->currentOutTemp);
   
   if ( temperature > 140 || temperature < 0 )
      return 0;
   if ( temperature <= DRYING_DONE_TEMP )
      GlobalState::instance()->state.heating_mode = HEATING_DRYING_STATE;
   else if ( temperature >= DRYING_DONE_TEMP && temperature < FRYING_DONE_TEMP )
      GlobalState::instance()->state.heating_mode = HEATING_FRYING_STATE;
   else if ( temperature >= FRYING_DONE_TEMP && temperature < FULL_DONE_TEMP )
      GlobalState::instance()->state.heating_mode = HEATING_BOILING_STATE;
   else 
      GlobalState::instance()->state.heating_mode = HEATING_NONE_STATE;
   
   GlobalState::instance()->currentProbeTemp = temperature;
   return temperature;
}

void ProbeThread::run()
{
   tempValue = getTemp();
   GlobalState::instance()->currentProbeTemp = tempValue;
   delay(100);
   runned();
}