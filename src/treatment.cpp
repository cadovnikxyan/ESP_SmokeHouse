#include "treatment.h"

DECLARATE_INSTANCE(HeatTreatmentThread)
HeatTreatmentThread::HeatTreatmentThread()
   :
     powerControlThread(PowerControlThread::instance()),
     dallasTempThread(DallasTempThread::instance()),
     probeThread(ProbeThread::instance()),
     controller(new ThreadController()),
     pid(nullptr),
     start(0),
     stop(0),
     startingFlag(false)
{
      controller->add(powerControlThread);
      controller->add(probeThread);
      controller->add(dallasTempThread);     
}

void HeatTreatmentThread::setUpPID()
{
   if ( pid != nullptr )
      delete pid;
   
   pid = new PID(dallasTempThread->getResultTemp(), powerControlThread->getPowerValueRef(), &setPoint, consKp, consKi, consKd, DIRECT);
   pid->SetOutputLimits(0, 853);
}

void HeatTreatmentThread::adjustPID()
{
   double gap = abs(setPoint - *dallasTempThread->getResultTemp()); 
   if ( gap < 10 )
      pid->SetTunings(consKp, consKi, consKd);
   else
      pid->SetTunings(aggKp, aggKi, aggKd);
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