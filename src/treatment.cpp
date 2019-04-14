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
     startingFlag(false),
     processStarted(false),
     setPoint(DRYING_OUT_TEMP)
{
      controller->add(powerControlThread);
      controller->add(probeThread);
      controller->add(dallasTempThread);     
}

void HeatTreatmentThread::setUpPID()
{
   if ( pid != nullptr )
      delete pid;
   
   pid = new PID(dallasTempThread->getResultTemp(), powerControlThread->getPowerValuePtr(), &setPoint, consKp, consKi, consKd, DIRECT);
   pid->SetOutputLimits(0, OUT_MAX_VALUE_PID);
}

void HeatTreatmentThread::adjustPID()
{
   double gap = abs(setPoint - *dallasTempThread->getResultTemp()); 
   if ( gap < 5 )
      pid->SetTunings(consKp, consKi, consKd);
   else
      pid->SetTunings(aggKp, aggKi, aggKd);
}

void HeatTreatmentThread::switchHeatingMode()
{
   
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
   if( startingFlag && !processStarted)
   {
      startTreatment();
      controller->run();
      if ( __globalState__.state & AUTO_MODE )
         setUpPID(); 

      powerControlThread->setConvection(true);
      processStarted = true;
   }

   else if ( !startingFlag && processStarted )
   {
      stopTreatment();
   }

   if ( processStarted )
   {
      if (__globalState__.currentProbeTemp >= DRYING_DONE_TEMP )
      {
         setPoint = FRYING_OUT_TEMP;
         powerControlThread->setAirPump(true);
      }
      else if (__globalState__.currentProbeTemp >= FRYING_DONE_TEMP )
      {
         setPoint = BOILING_OUT_TEMP;
         powerControlThread->setWaterPump(true);
      }
      else if ( __globalState__.currentProbeTemp >= FULL_DONE_TEMP )
      {
         processStarted = false;
         startingFlag = false;
      }
      adjustPID();
      pid->Compute();
   }
   runned();
}

String HeatTreatmentThread::setConvection(bool state)
{
    PowerControlThread::instance()->setConvection(state);
    return "true";
}

String HeatTreatmentThread::setHeating(bool state)
{
   PowerControlThread::instance()->setHeating();
   return "true";
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

String HeatTreatmentThread::getCurrentState() const
{
   return getJsonState();
}

String HeatTreatmentThread::getJsonState() const
{
   StaticJsonDocument<400> jsonState;
   JsonObject root = jsonState.to<JsonObject>();

   root["mode"] = __globalState__.getMode();
   root["heatingState"] = __globalState__.getHeatingState();
   root["heatingMode"] = __globalState__.getHeatingMode();
   root["convectionState"] = __globalState__.getConvectionState();
   root["airPumpState"] = __globalState__.getAirPumpState();
   root["waterPumpState"] = __globalState__.getWaterPumpState();

   root["currentOutTemp"] = __globalState__.currentOutTemp;
   root["currentProbeTemp"] = __globalState__.currentProbeTemp;
   String result;
   serializeJson(root, result);
   Serial.println(__globalState__.state, 16);
   return result;
}