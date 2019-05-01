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

void HeatTreatmentThread::startTreatment()
{
   start = time(nullptr);
}

void HeatTreatmentThread::stopTreatment()
{
   stop = time(nullptr);
}

void HeatTreatmentThread::autoMode() 
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
      if( pid != nullptr )
      {
         adjustPID();
         pid->Compute();
      }
   }
}

void HeatTreatmentThread::manualMode() 
{

}

void HeatTreatmentThread::smokingMode() 
{

}

void HeatTreatmentThread::noHeatingMode() 
{

}

void HeatTreatmentThread::run() 
{
   if ( __globalState__.state & AUTO_MODE )
      autoMode();
   else if ( __globalState__.state & MANUAL_MODE )
      manualMode();
   else if ( __globalState__.state & SMOKING_MODE )
      smokingMode();
   else if ( __globalState__.state & NO_HEATING )
      noHeatingMode();

   runned();
}

String HeatTreatmentThread::setState(String stringState)
{     
   DynamicJsonDocument root(400);
   deserializeJson(root, stringState);

   __globalState__.setMode(root["mode"].as<String>());
   __globalState__.setHeatingMode(root["heatingMode"].as<String>());
   __globalState__.setHeatingState(root["heatingState"].as<bool>());
   __globalState__.setConvectionState(root["convectionState"].as<bool>());
   __globalState__.setAirPumpState(root["airPumpState"].as<bool>());
   __globalState__.setWaterPumpState(root["waterPumpState"].as<bool>());

   setStart(root["startHeatTreatment"].as<bool>());
   return getJsonState();
}

String HeatTreatmentThread::getJsonTemps() const
{
   StaticJsonDocument<200> jsonTemp;
   StaticJsonDocument<200> jsonProbe;
   String temp = DallasTempThread::instance()->getJsonTemp();
   Serial.println(temp);
   String probe = ProbeThread::instance()->getJsonProbeTemp();
   deserializeJson(jsonTemp, temp);
   deserializeJson(jsonProbe, probe);
   StaticJsonDocument<400> root;
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
   StaticJsonDocument<400> root;

   root["mode"] = __globalState__.getMode();
   root["heatingState"] = __globalState__.getHeatingState();
   root["heatingMode"] = __globalState__.getHeatingMode();
   root["convectionState"] = __globalState__.getConvectionState();
   root["airPumpState"] = __globalState__.getAirPumpState();
   root["waterPumpState"] = __globalState__.getWaterPumpState();

   root["currentOutTemp"] = __globalState__.currentOutTemp;
   root["currentProbeTemp"] = __globalState__.currentProbeTemp;
   root["started"] = getStartFlag();
   String result;
   serializeJson(root, result);
   return result;
}