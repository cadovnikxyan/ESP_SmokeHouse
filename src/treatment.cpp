#include "treatment.h"

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


DECLARATE_INSTANCE(DallasTempThread)
DallasTempThread::DallasTempThread()
{
   tempWire = new OneWire(TEMP_PIN);
   temp = new DallasTemperature(tempWire);
   temp->begin();
}

DallasTempThread::~DallasTempThread()
{
   delete temp;
   delete tempWire;
}

String DallasTempThread::getJsonTemp()
{
   StaticJsonDocument<200> jsonBuffer;
   JsonObject root = jsonBuffer.to<JsonObject>();
   root["temp1"] = tempValue1;
   root["temp2"] = tempValue2;
   root["temp3"] = tempValue3;
   String result;
   serializeJson(root, result);
   return result;
}

float DallasTempThread::calculateAverage() const
{
   return (tempValue1 + tempValue2 + tempValue3) / 3;
}

void DallasTempThread::run()
{
   temp->requestTemperatures();
   tempValue1 = temp->getTempCByIndex(0);
   tempValue2 = temp->getTempCByIndex(1);
   tempValue3 = temp->getTempCByIndex(2);
   runned();
}

const static int relays[] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4, RELAY_PIN_5, RELAY_PIN_6};

DECLARATE_INSTANCE(PowerControlThread)
PowerControlThread::PowerControlThread()
{
   for ( auto i : relays )
      pinMode(i, OUTPUT);
}

void PowerControlThread::onRelay(int relayPin, int state)
{
   digitalWrite(relayPin, state);
}

String PowerControlThread::setHeating(bool state)
{
   if ( state )
   {
      onRelay(TEH_TOP, HIGH);
      onRelay(TEH_BOTTOM, HIGH);
   }
   else
   {
      onRelay(TEH_TOP, LOW);
      onRelay(TEH_BOTTOM, LOW);
   }
   
   return "true";
}

String PowerControlThread::setConvection(bool state)
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
   
   return "true";
}

String PowerControlThread::setWaterPump(bool state)
{
   if ( state )
      onRelay(WATER_PUMP, HIGH);
   else 
      onRelay(WATER_PUMP, LOW);

   return "true";
}

String PowerControlThread::setAirPump(bool state)
{
   if ( state )
      onRelay(AIR_PUMP, HIGH);
   else
      onRelay(AIR_PUMP, LOW);
   return "true";
}

String PowerControlThread::setMode(const String& mode)
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
      return "false";
   }
   return "true";
}

  uint8_t PowerControlThread::getCurrentMode() const
  {
     return currentMode;
  }


void PowerControlThread::run()
{
   runned();
}

DECLARATE_INSTANCE(HeatTreatmentThread)
HeatTreatmentThread::HeatTreatmentThread()
   :
     powerControlThread(PowerControlThread::instance()),
     dallasTempThread(DallasTempThread::instance()),
     probeThread(ProbeThread::instance()),
     controller(new ThreadController()),
     start(0),
     stop(0),
     startingFlag(false)
{
      controller->add(powerControlThread);
      controller->add(probeThread);
      controller->add(dallasTempThread);     
}

void HeatTreatmentThread::startTreatment()
{
   start = time(nullptr);
}

void HeatTreatmentThread::stopTreatment()
{
   stop = time(nullptr);
}

void HeatTreatmentThread::run() 
{
   if( startingFlag )
   {
      startTreatment();
      controller->run();
   }
   else 
   {
      stopTreatment();
   }

   runned();
}

String HeatTreatmentThread::setConvection(bool state)
{
   return PowerControlThread::instance()->setConvection(state);
}

String HeatTreatmentThread::setHeating(bool state)
{
   return PowerControlThread::instance()->setHeating(state);
}

String HeatTreatmentThread::setAirPump(bool state)
{
   return PowerControlThread::instance()->setAirPump(state);
}

String HeatTreatmentThread::setWaterPump(bool state)
{
   return PowerControlThread::instance()->setWaterPump(state);
}

String HeatTreatmentThread::getJsonTemps() const
{
   StaticJsonDocument<200> jsonTemp;
   StaticJsonDocument<200> jsonProbe;
   String temp = DallasTempThread::instance()->getJsonTemp();
   String probe = ProbeThread::instance()->getJsonProbeTemp();
   deserializeJson(jsonTemp, temp);
   deserializeJson(jsonProbe, probe);
   StaticJsonDocument<400> jsonBuffer;
   JsonObject root = jsonBuffer.to<JsonObject>();
   root["temp"] = jsonTemp.to<JsonObject>();
   root["probe"] = jsonProbe.to<JsonObject>();
   String result;
   serializeJson(root, result);
   return result;
}

void HeatTreatmentThread::setStart(bool flag)
{
   startingFlag = flag;
}

bool HeatTreatmentThread::getStartFlag() const
{
   return startingFlag;
}

String HeatTreatmentThread::setMode(const String& mode)
{
   return PowerControlThread::instance()->setMode(mode);
}

String HeatTreatmentThread::getCurrentState() const
{
   return "{state: none}";
}

void HeatTreatmentThread::setSettings(const String& settings)
{

}