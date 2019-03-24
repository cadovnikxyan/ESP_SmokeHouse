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

String PowerControlThread::setHeatingOn()
{
   onRelay(RELAY_PIN_1, HIGH);
   return "true";
}

String PowerControlThread::setHeatingOff()
{
   onRelay(RELAY_PIN_1, LOW);
   return "true";
}

String PowerControlThread::setConvectionOn()
{
   onRelay(RELAY_PIN_2, HIGH);
   onRelay(RELAY_PIN_3, HIGH);
   return "true";
}

String PowerControlThread::setConvectionOff()
{
   onRelay(RELAY_PIN_2, LOW);
   onRelay(RELAY_PIN_3, LOW);
   return "true";
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

String HeatTreatmentThread::setConvectionOff()
{
   return PowerControlThread::instance()->setConvectionOff();
}

String HeatTreatmentThread::setConvectionOn()
{
   return PowerControlThread::instance()->setConvectionOn();
}

String HeatTreatmentThread::setHeatingOff()
{
   return PowerControlThread::instance()->setHeatingOff();
}

String HeatTreatmentThread::setHeatingOn()
{
   return PowerControlThread::instance()->setHeatingOn();
}

String HeatTreatmentThread::getJsonProbeTemp()
{
   return ProbeThread::instance()->getJsonProbeTemp();
}

String HeatTreatmentThread::getJsonTemp()
{
   return DallasTempThread::instance()->getJsonTemp();
}

void HeatTreatmentThread::setStart(bool flag)
{
   startingFlag = flag;
}

bool HeatTreatmentThread::getStartFlag() const
{
   return startingFlag;
}